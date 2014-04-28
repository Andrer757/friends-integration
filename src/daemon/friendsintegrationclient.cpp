/*
 * Copyright (C) 2014 Lucien XU <sfietkonstantin@free.fr>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * The names of its contributors may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include "friendsintegrationclient.h"
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QSettings>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <notification.h>
#include "../../friends/src/defines_p.h"
#include "../../social/src/facebook/facebookontology_p.h"
#include "friendsjsonpostdatabase.h"
#include "constants_p.h"

static const char *FRIENDS_PROVIDER = "friends";
static const char *FRIENDS_SYNC_SERVICE = "friends-sync";

// From Friends (copy paste)
static const char *QUERY = "{"
    "\"query1\": \"SELECT actor_id, attachment, comment_info, created_time, description,"
                  "description_tags,is_hidden,is_published,like_info,message,message_tags,"
                  "parent_post_id,permalink,place,share_info,"
                  "tagged_ids,target_id,post_id,updated_time,with_tags "
                  "FROM stream "
                  "%1 "
                  "%2 "
                  "ORDER BY created_time DESC LIMIT 30\","
    "\"query2\": \"SELECT uid,name FROM user WHERE uid in (SELECT actor_id,target_id FROM #query1)\","\
    "\"query3\": \"SELECT page_id,name FROM page WHERE page_id in (SELECT actor_id,target_id FROM #query1)\","\
    "\"query4\": \"SELECT gid,name FROM group WHERE gid in (SELECT actor_id,target_id FROM #query1)\","\
    "\"query5\": \"SELECT eid,name FROM event WHERE eid in (SELECT actor_id,target_id FROM #query1)\"}";
static const char *QUERY_QUERY1 = "query1";
static const char *QUERY_QUERY2 = "query2";
static const char *QUERY_QUERY3 = "query3";
static const char *QUERY_QUERY4 = "query4";
static const char *QUERY_QUERY5 = "query5";
static const char *QUERY_DATA_KEY = "data";
static const char *QUERY_NAME_KEY = "name";
static const char *QUERY_RESULT_KEY = "fql_result_set";
static const char *QUERY_UID_KEY = "uid";
static const char *QUERY_PAGEID_KEY = "page_id";
static const char *QUERY_GID_KEY = "gid";
static const char *QUERY_EID_KEY = "eid";

static const char *WALL_WHERE = "WHERE filter_key in "
                                "    (SELECT filter_key FROM stream_filter "
                                "     WHERE uid = me() AND type = 'newsfeed') ";


FriendsIntegrationClient::FriendsIntegrationClient(const QString& pluginName,
                                 const Buteo::SyncProfile& profile,
                                 Buteo::PluginCbInterface *cbInterface)
    : ClientPlugin(pluginName, profile, cbInterface)
    , m_tasks(0)
    , m_manager(new Accounts::Manager(this))
    , m_network(new QNetworkAccessManager(this))
    , m_notificationsReply(0)
    , m_postsReply(0)
    , m_db(new FriendsJsonPostDatabase())
{
    QSettings settings (ORGANIZATION_NAME, APPLICATION_NAME);
    m_token = settings.value("login/token").toString();
}

FriendsIntegrationClient::~FriendsIntegrationClient()
{
    m_db->deleteLater();
}

bool FriendsIntegrationClient::init()
{
    if (m_token.isEmpty()) {
        qWarning() << "Syncing without access token";
        return false;
    }

    return true;
}

bool FriendsIntegrationClient::uninit()
{
    return true;
}

bool FriendsIntegrationClient::startSync()
{
    // Select the account
    QList<Accounts::Account *> accounts;
    foreach (const Accounts::AccountId id, m_manager->accountList()) {
        Accounts::Account *account = m_manager->account(id);
        if (account->providerName() == FRIENDS_PROVIDER) {
            accounts.append(account);
        }
    }

    qDebug() << "Found accounts" << accounts.count();

    if (accounts.count() != 1) {
        finish();
        return true;
    }

    bool enabled = false;
    Accounts::Account *account = accounts.first();
    foreach (const Accounts::Service &service, account->enabledServices()) {
        if (service.name() == FRIENDS_SYNC_SERVICE) {
            enabled = true;
            break;
        }
    }

    if (enabled) {
        qDebug() << "Start sync: account" << account->id();

        syncNotifications();
        syncFeed();
    } else {
        emit success(getProfileName(), "Sync finished");
    }
    return true;
}

void FriendsIntegrationClient::abortSync(Sync::SyncStatus status)
{
    Q_UNUSED(status)
    if (m_notificationsReply) {
        m_notificationsReply->deleteLater();
        m_notificationsReply = 0;
    }

    if (m_postsReply) {
        m_postsReply->deleteLater();
        m_postsReply = 0;
    }
}

Buteo::SyncResults FriendsIntegrationClient::getSyncResults() const
{
    return m_results;
}

bool FriendsIntegrationClient::cleanUp()
{
    if (m_notificationsReply) {
        m_notificationsReply->deleteLater();
        m_notificationsReply = 0;
    }

    if (m_postsReply) {
        m_postsReply->deleteLater();
        m_postsReply = 0;
    }

    return true;
}

void FriendsIntegrationClient::connectivityStateChanged(Sync::ConnectivityType type, bool state)
{
    Q_UNUSED(type)
    Q_UNUSED(state)
}

void FriendsIntegrationClient::syncNotifications()
{
    startTask();
    QUrl url ("https://graph.facebook.com/me/notifications");
    QUrlQuery urlQuery (url);
    urlQuery.addQueryItem("include_read", "1");
    urlQuery.addQueryItem("fields", "id,created_time,unread");
    urlQuery.addQueryItem("limit", "50");
    urlQuery.addQueryItem("access_token", m_token);
    url.setQuery(urlQuery);
    m_notificationsReply = m_network->get(QNetworkRequest(url));

    connect(m_notificationsReply, &QNetworkReply::finished,
            this, &FriendsIntegrationClient::slotNotificationsFinished);
}

void FriendsIntegrationClient::syncFeed()
{
    startTask();

    // Build our query
    QString query = QString(QUERY).arg(WALL_WHERE, QString());
    QUrl url ("https://graph.facebook.com/fql");
    QUrlQuery urlQuery (url);
    urlQuery.addQueryItem("q", query);
    urlQuery.addQueryItem("access_token", m_token);
    url.setQuery(urlQuery);

    m_postsReply = m_network->get(QNetworkRequest(url));

    connect(m_postsReply, &QNetworkReply::finished,
            this, &FriendsIntegrationClient::slotFeedFinished);


}

void FriendsIntegrationClient::finish()
{
    emit success(getProfileName(), "Sync finished");
}

void FriendsIntegrationClient::startTask()
{
    m_tasks ++;
}

void FriendsIntegrationClient::finishTask()
{
    if (m_tasks > 0) {
        m_tasks --;

        if (m_tasks == 0) {
            finish();
        }
    }
}

void FriendsIntegrationClient::slotNotificationsFinished()
{
    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(m_notificationsReply->readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        finishTask();
        return;
    }

    int newNotifications = 0;

    QSettings settings (ORGANIZATION_NAME, APPLICATION_NAME);
    QString notificationsTimestampStr = settings.value("sync/notificationsTimestamp").toString();
    QDateTime notificationsTimestamp = QDateTime::fromString(notificationsTimestampStr, Qt::ISODate);
    qDebug() << "Last notifications sync" << notificationsTimestamp;
    QJsonArray array = document.object().value("data").toArray();
    foreach (const QJsonValue &value, array) {
        QJsonObject notification = value.toObject();
        QString timestampStr = notification.value("created_time").toString();
        QDateTime timestamp = QDateTime::fromString(timestampStr, Qt::ISODate);
        if (timestamp > notificationsTimestamp) {
            newNotifications ++;
        }
    }

    notificationsTimestampStr = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    settings.setValue("sync/notificationsTimestamp", notificationsTimestampStr);

    if (newNotifications == 0) {
        finishTask();
        return;
    }

    Notification *notification = 0;
    QList<QObject *> notifications = Notification::notifications();
    foreach (QObject *object, notifications) {
        Notification *castedNotification = static_cast<Notification *>(object);
        if (castedNotification->category() == "x-nemo.social.facebook.notification"
            && castedNotification->hintValue("x-nemo.friendsintegration").toBool()) {
            notification = castedNotification;
            break;
        }
    }

    if (!notification) {
        notification = new Notification(this);
        notification->setCategory("x-nemo.social.facebook.notification");
        notification->setHintValue("x-nemo.friendsintegration", true);
    }

    newNotifications += notification->itemCount();
    QString body = "Friends";
    QString summary = tr("You have %n notifications", "", newNotifications);

    notification->setSummary(summary);
    notification->setBody(body);
    notification->setPreviewSummary(summary);
    notification->setPreviewBody(body);
    notification->setItemCount(newNotifications);
    notification->setTimestamp(QDateTime::currentDateTime());
    notification->setRemoteDBusCallServiceName("harbour.friends");
    notification->setRemoteDBusCallObjectPath("/");
    notification->setRemoteDBusCallInterface("harbour.friends");
    notification->setRemoteDBusCallMethodName("openNotifications");
    notification->setRemoteDBusCallArguments(QVariantList());
    notification->publish();

    finishTask();
}

void FriendsIntegrationClient::slotFeedFinished()
{
    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(m_postsReply->readAll(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        finishTask();
        return;
    }

    QJsonObject parsed = document.object();
    if (!parsed.contains(QUERY_DATA_KEY)) {
        finishTask();
        return;
    }

    QJsonArray dataList = parsed.value(QLatin1String("data")).toArray();

    // Cache names
    QMap<QString, QString> names;
    QJsonArray mainData;
    foreach (const QJsonValue entry, dataList) {
        QJsonObject entryObject = entry.toObject();
        QString name = entryObject.value(QUERY_NAME_KEY).toString();
        if (name == QUERY_QUERY1) {
            mainData = entryObject.value(QUERY_RESULT_KEY).toArray();
        } else if (name == QUERY_QUERY2
                   || name == QUERY_QUERY3
                   || name == QUERY_QUERY4
                   || name == QUERY_QUERY5) {
            QString key;
            if (name == QUERY_QUERY2) {
                key = QUERY_UID_KEY;
            } else if(name == QUERY_QUERY3) {
                key = QUERY_PAGEID_KEY;
            } else if(name == QUERY_QUERY4) {
                key = QUERY_GID_KEY;
            } else if(name == QUERY_QUERY5) {
                key = QUERY_EID_KEY;
            }

            QJsonArray nameList = entryObject.value(QUERY_RESULT_KEY).toArray();
            foreach (const QJsonValue &name, nameList) {
                QJsonObject nameObject = name.toObject();
                names.insert(nameObject.value(key).toVariant().toString(),
                             nameObject.value(QUERY_NAME_KEY).toString());
            }
        }
    }

    // Filter meaningless data
    QSet<QString> postObjectIds;
    QList<QJsonObject> postObjects;
    foreach (QJsonValue entry, mainData) {
        QJsonObject post = entry.toObject();
        QString postId = post.value("post_id").toVariant().toString();
        if (!postObjectIds.contains(postId)) {
            postObjectIds.insert(postId);
            postObjects.append(post);
        }
    }

    foreach (const QJsonObject &postMap, postObjects) {
        // Any post with a parent_post_id will be discarded if that parent_post_id
        // is contained in the list of primary post ids, as we already cache that parent post,
        // unless the post_id is the same as the parent_post_id.
        QString postId = postMap.value("post_id").toString();
        QString parentPostId = postMap.value("parent_post_id").toString();
        if (!parentPostId.isEmpty() && postObjectIds.contains(parentPostId) && parentPostId != postId) {
            continue;
        }

        // Discarding not published posts
        if (!postMap.value("is_published").toBool()) {
            continue;
        }

        // Discarding posts without a story and without a message
        QString message = postMap.value(FACEBOOK_ONTOLOGY_POST_MESSAGE).toString();
        QString story = postMap.value("description").toString();
        if (message.isEmpty() && story.isEmpty()) {
            continue;
        }

        // Prepare attachment management:
        QJsonObject attachment = postMap.value("attachment").toObject();

        bool hasMedia = false;
        bool isVideo = false;

        QJsonArray mediaList;
        // If media was provided, we need to ensure that it's valid, else discard the post.
        if (attachment.keys().contains("media") && !attachment.value("media").isNull()) {
            hasMedia = true;
            bool wrongMediaFound = false;
            QJsonArray media = attachment.value("media").toArray();

            foreach (QJsonValue medium, media) {
                QJsonObject mediumMap = medium.toObject();
                if (mediumMap.contains("video")) {
                    isVideo = true;
                }
                QString mediaUrlString = mediumMap.value("src").toString();

                // Skip those media that do not have URL (someone went to an event)
                if (mediaUrlString.isEmpty()) {
                    wrongMediaFound = true;
                    continue;
                }

                // Try to find a better image for this media
                if (mediumMap.contains("photo")) {
                    QJsonArray imageList = mediumMap.value("photo").toObject().value("images").toArray();
                    if (!imageList.isEmpty()) {
                        QString newImage = imageList.last().toObject().value("src").toString();
                        if (!newImage.isEmpty()) {
                            mediaUrlString = newImage;
                        }
                    }
                }

                // Patch an issue with some applications using local path instead
                // of absolute urls
                if (!mediaUrlString.startsWith("http")) {
                    mediaUrlString.prepend("https://facebook.com/");
                }

                QString urlString = QUrl::fromEncoded(mediaUrlString.toLocal8Bit()).toString();
                mediaList.append(urlString);
            }

            if (wrongMediaFound) {
                continue;
            }
        }

        // Discard stories without attachments
        if (!hasMedia && !story.isEmpty() && message.isEmpty()) {
            continue;
        }

        QUrl source;
        if (attachment.contains("href")) {
            source = QUrl(attachment.value("href").toString());
        }

        QJsonObject postData;
        postData.insert(FACEBOOK_ONTOLOGY_METADATA_ID, postId);

        // Extra fields
        postData.insert(MEDIA_KEY, mediaList);
        postData.insert(IS_VIDEO_KEY, isVideo);

        // From
        QString fromId = postMap.value("actor_id").toVariant().toString();
        QString fromName = names.value(fromId);
        QJsonObject fromData;
        fromData.insert(FACEBOOK_ONTOLOGY_OBJECTREFERENCE_OBJECTIDENTIFIER, fromId);
        fromData.insert(FACEBOOK_ONTOLOGY_OBJECTREFERENCE_OBJECTNAME, fromName);
        postData.insert(FACEBOOK_ONTOLOGY_POST_FROM, fromData);

        // To
        // If to is not null, there is usually one target ...
        QString toId = postMap.value("target_id").toString();
        if (!toId.isEmpty()) {
            QString toName = names.value(toId);
            QJsonObject toData;
            toData.insert(FACEBOOK_ONTOLOGY_OBJECTREFERENCE_OBJECTIDENTIFIER, toId);
            toData.insert(FACEBOOK_ONTOLOGY_OBJECTREFERENCE_OBJECTNAME, toName);
            postData.insert(FACEBOOK_ONTOLOGY_POST_TO, toData);
        }

        // Message
        postData.insert(FACEBOOK_ONTOLOGY_POST_MESSAGE, message);
        // Message tags
        postData.insert(FACEBOOK_ONTOLOGY_POST_MESSAGETAGS,
                        postMap.value(FACEBOOK_ONTOLOGY_POST_MESSAGETAGS));

        // Picture is not used so we don't set it.
        QString attachmentName = attachment.value("name").toString();
        QString attachmentCaption = attachment.value("caption").toString();
        QString attachmentDescription = attachment.value("description").toString();
        QString attachmentUrl = attachment.value("href").toString();

        // Facebook object id / type
        postData.insert(FACEBOOK_OBJECT_ID, attachment.value(FACEBOOK_OBJECT_ID));
        postData.insert(FACEBOOK_OBJECT_TYPE, attachment.value(FACEBOOK_OBJECT_TYPE));

        // Link
        postData.insert(FACEBOOK_ONTOLOGY_POST_LINK, attachmentUrl);

        // Name
        postData.insert(FACEBOOK_ONTOLOGY_POST_NAME, attachmentName);

        // Caption
        postData.insert(FACEBOOK_ONTOLOGY_POST_CAPTION, attachmentCaption);

        // Story
        postData.insert(FACEBOOK_ONTOLOGY_POST_DESCRIPTION, attachmentDescription);

        // Source
        if (!source.host().contains("facebook.com")) {
            postData.insert(FACEBOOK_ONTOLOGY_POST_SOURCE, source.toString());
        } else {
            postData.insert(FACEBOOK_ONTOLOGY_POST_SOURCE, QString());
        }
        // Source is not used so we don't set it.
        // Properties is not used so we don't set it.
        // Icon is not used so we don't set it.
        // Actions is not queried (TODO ?)
        // postType is not reliable
        // Story
        postData.insert(FACEBOOK_ONTOLOGY_POST_STORY, story);

        // Story tags
        postData.insert(FACEBOOK_ONTOLOGY_POST_STORYTAGS, postMap.value("description_tags"));

        // With tags
        postData.insert(FACEBOOK_ONTOLOGY_POST_WITHTAGS,
                        postMap.value(FACEBOOK_ONTOLOGY_POST_WITHTAGS));

        // ObjectIdentifier is not used so we don't set it.
        // Application is not queried (TODO ?)
        // CreatedTime
        uint createdTimestamp = postMap.value(FACEBOOK_ONTOLOGY_POST_CREATEDTIME).toVariant().toUInt();
        QDateTime createdTime = QDateTime::fromTime_t(createdTimestamp);
        createdTime = createdTime.toTimeSpec(Qt::UTC);
        postData.insert(FACEBOOK_ONTOLOGY_POST_CREATEDTIME, createdTime.toString(Qt::ISODate));

        // UpdatedTime
        uint updatedTimestamp = postMap.value(FACEBOOK_ONTOLOGY_POST_UPDATEDTIME).toVariant().toUInt();
        QDateTime updatedTime = QDateTime::fromTime_t(updatedTimestamp);
        updatedTime = updatedTime.toTimeSpec(Qt::UTC);
        postData.insert(FACEBOOK_ONTOLOGY_POST_UPDATEDTIME, updatedTime.toString(Qt::ISODate));

        // Shares
        QJsonObject shareInfo = postMap.value("share_info").toObject();
        int shares = shareInfo.value("share_count").toVariant().toInt();
        postData.insert(FACEBOOK_ONTOLOGY_POST_SHARES, shares);

        // Hidden
        bool hidden = postMap.value(FACEBOOK_ONTOLOGY_POST_HIDDEN).toBool();
        postData.insert(FACEBOOK_ONTOLOGY_POST_HIDDEN, hidden);

        // StatusType is not used so we don't set it.
        // Likes
//        QJsonObject likeInfo = postMap.value("like_info").toObject();
//        QVariantMap likesData;
//        QVariantMap likesSummary;
//        likesSummary.insert(FACEBOOK_ONTOLOGY_METADATA_TOTALCOUNT,
//                            likeInfo.value("like_count").toInt());
//        QVariantList likesObjects;
//        if (likeInfo.value("user_likes").toBool()) {
//            QVariantMap me;
//            me.insert(FACEBOOK_ONTOLOGY_OBJECTREFERENCE_OBJECTIDENTIFIER, currentUserIdentifier);
//            likesObjects.append(me);
//        }
//        likesData.insert(FACEBOOK_ONTOLOGY_METADATA_SUMMARY, likesSummary);
//        likesData.insert(FACEBOOK_ONTOLOGY_METADATA_DATA, likesObjects);
//        postData.insert(FACEBOOK_ONTOLOGY_CONNECTIONS_LIKES, likesData);
        // Comments
//        QJsonObject commentInfo = postMap.value("comment_info").toObject();
//        QVariantMap commentData;
//        QVariantMap commentSummary;
//        commentSummary.insert(FACEBOOK_ONTOLOGY_METADATA_TOTALCOUNT,
//                            commentInfo.value("comment_count").toInt());
//        commentData.insert(FACEBOOK_ONTOLOGY_METADATA_SUMMARY, commentSummary);
//        postData.insert(FACEBOOK_ONTOLOGY_CONNECTIONS_COMMENTS, commentData);

        QJsonDocument document;
        document.setObject(postData);
        m_db->addPost(postId, document, createdTime);
    }

    m_db->commit();
    m_db->wait();


    finishTask();
}

extern "C" FriendsIntegrationClient* createPlugin(const QString& pluginName,
                                                  const Buteo::SyncProfile& profile,
                                                  Buteo::PluginCbInterface *cbInterface)
{
    return new FriendsIntegrationClient(pluginName, profile, cbInterface);
}

extern "C" void destroyPlugin(FriendsIntegrationClient *client)
{
    delete client;
}
