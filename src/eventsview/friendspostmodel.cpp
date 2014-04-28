/*
 * Copyright (C) 2013 Jolla Ltd.
 * Contact: Lucien Xu <lucien.xu@jollamobile.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "friendspostmodel.h"
#include "abstractsocialcachemodel_p.h"
#include "friendsjsonpostdatabase.h"
#include "friendspostinterface.h"
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include "../daemon/constants_p.h"
#include "../../social/src/facebook/facebookontology_p.h"

class FriendsPostModelPrivate: public AbstractSocialCacheModelPrivate
{
public:
    explicit FriendsPostModelPrivate(FriendsPostModel *q);
    FriendsJsonPostDatabase database;

private:
    Q_DECLARE_PUBLIC(FriendsPostModel)
};

FriendsPostModelPrivate::FriendsPostModelPrivate(FriendsPostModel *q)
    : AbstractSocialCacheModelPrivate(q)
{
}

FriendsPostModel::FriendsPostModel(QObject *parent)
    : AbstractSocialCacheModel(*(new FriendsPostModelPrivate(this)), parent)
{
    Q_D(FriendsPostModel);

    connect(&d->database, &FriendsJsonPostDatabase::postsChanged,
            this, &FriendsPostModel::postsChanged);
}

QHash<int, QByteArray> FriendsPostModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(IdentifierRole, "identifier");
    roleNames.insert(ContentItemRole, "contentItem");
    roleNames.insert(TimestampRole, "timestamp");
    return roleNames;
}

void FriendsPostModel::refresh()
{
    Q_D(FriendsPostModel);
    d->database.refresh();
}

void FriendsPostModel::postsChanged()
{
    Q_D(FriendsPostModel);

    SocialCacheModelData data;
    QList<QJsonDocument> postsData = d->database.posts();

    Q_FOREACH (const QJsonDocument &post, postsData) {
        QMap<int, QVariant> eventMap;
        QJsonObject object = post.object();
        QString identifier = object.value(FACEBOOK_ONTOLOGY_METADATA_ID).toString();
        QJsonObject from = object.value(FACEBOOK_ONTOLOGY_POST_FROM).toObject();
        QString fromIdentifier = from.value(FACEBOOK_ONTOLOGY_OBJECTREFERENCE_OBJECTIDENTIFIER).toString();
        QString fromName = from.value(FACEBOOK_ONTOLOGY_OBJECTREFERENCE_OBJECTNAME).toString();
        FriendsObjectReferenceInterface * fromObject = FriendsObjectReferenceInterface::create(fromIdentifier, fromName, this);
        QJsonArray to = object.value(FACEBOOK_ONTOLOGY_POST_TO).toArray();
        QList<FriendsObjectReferenceInterface *> toList;
        foreach (const QJsonValue &toEntry, to) {
            QJsonObject object = toEntry.toObject();
            QString identifier = object.value(FACEBOOK_ONTOLOGY_OBJECTREFERENCE_OBJECTIDENTIFIER).toString();
            QString name = object.value(FACEBOOK_ONTOLOGY_OBJECTREFERENCE_OBJECTNAME).toString();
            toList.append(FriendsObjectReferenceInterface::create(identifier, name, this));
        }

        QString message = object.value(FACEBOOK_ONTOLOGY_POST_MESSAGE).toString();
        QJsonArray messageTags = object.value(FACEBOOK_ONTOLOGY_POST_MESSAGETAGS).toArray();
        QList<FriendsNameTagInterface *> messageTagsList;
        foreach (const QJsonValue &messageTagsEntry, messageTags) {
            QJsonObject object = messageTagsEntry.toObject();
            QString userIdentifier = object.value(FACEBOOK_ONTOLOGY_NAME_TAG_USERIDENTIFIER).toString();
            QString userName = object.value(FACEBOOK_ONTOLOGY_NAME_TAG_USERNAME).toString();
            QString nameTagType = object.value(FACEBOOK_ONTOLOGY_NAME_TAG_NAMETAGTYPE).toString();
            int offset = object.value(FACEBOOK_ONTOLOGY_NAME_TAG_OFFSET).toVariant().toInt();
            int length = object.value(FACEBOOK_ONTOLOGY_NAME_TAG_LENGTH).toVariant().toInt();
            messageTagsList.append(FriendsNameTagInterface::create(userIdentifier, userName, nameTagType, offset, length, this));
        }

        QString link = object.value(FACEBOOK_ONTOLOGY_POST_LINK).toString();
        QString name = object.value(FACEBOOK_ONTOLOGY_POST_NAME).toString();
        QString caption = object.value(FACEBOOK_ONTOLOGY_POST_CAPTION).toString();
        QString description = object.value(FACEBOOK_ONTOLOGY_POST_DESCRIPTION).toString();
        QString source = object.value(FACEBOOK_ONTOLOGY_POST_SOURCE).toString();
        QString story = object.value(FACEBOOK_ONTOLOGY_POST_STORY).toString();
        QJsonArray storyTags = object.value(FACEBOOK_ONTOLOGY_POST_STORYTAGS).toArray();
        QList<FriendsNameTagInterface *> storyTagsList;
        foreach (const QJsonValue &storyTagsEntry, storyTags) {
            QJsonObject object = storyTagsEntry.toObject();
            QString userIdentifier = object.value(FACEBOOK_ONTOLOGY_NAME_TAG_USERIDENTIFIER).toString();
            QString userName = object.value(FACEBOOK_ONTOLOGY_NAME_TAG_USERNAME).toString();
            QString nameTagType = object.value(FACEBOOK_ONTOLOGY_NAME_TAG_NAMETAGTYPE).toString();
            int offset = object.value(FACEBOOK_ONTOLOGY_NAME_TAG_OFFSET).toVariant().toInt();
            int length = object.value(FACEBOOK_ONTOLOGY_NAME_TAG_LENGTH).toVariant().toInt();
            storyTagsList.append(FriendsNameTagInterface::create(userIdentifier, userName, nameTagType, offset, length, this));
        }
        QJsonArray withTags = object.value(FACEBOOK_ONTOLOGY_POST_WITHTAGS).toArray();
        QList<FriendsObjectReferenceInterface *> withTagsList;
        foreach (const QJsonValue &withTagsEntry, withTags) {
            QJsonObject object = withTagsEntry.toObject();
            QString identifier = object.value(FACEBOOK_ONTOLOGY_OBJECTREFERENCE_OBJECTIDENTIFIER).toString();
            QString name = object.value(FACEBOOK_ONTOLOGY_OBJECTREFERENCE_OBJECTNAME).toString();
            withTagsList.append(FriendsObjectReferenceInterface::create(identifier, name, this));
        }

        QString createdTime = object.value(FACEBOOK_ONTOLOGY_POST_CREATEDTIME).toString();
        QString updatedTime = object.value(FACEBOOK_ONTOLOGY_POST_UPDATEDTIME).toString();
        int shares = object.value(FACEBOOK_ONTOLOGY_POST_SHARES).toVariant().toInt();
        bool hidden = object.value(FACEBOOK_ONTOLOGY_POST_HIDDEN).toBool();

        QJsonArray mediaList = object.value(MEDIA_KEY).toArray();
        bool isVideo = object.value(IS_VIDEO_KEY).toBool();

        QStringList mediaStringList;
        foreach (const QJsonValue &mediaEntry, mediaList) {
            mediaStringList.append(mediaEntry.toString());
        }


        eventMap.insert(IdentifierRole, identifier);
        FriendsPostInterface *post = FriendsPostInterface::create(fromObject,
                                                                  toList,
                                                                  message,
                                                                  messageTagsList,
                                                                  QUrl(),
                                                                  QUrl(link),
                                                                  name,
                                                                  caption,
                                                                  description,
                                                                  source,
                                                                  QString(),
                                                                  story,
                                                                  storyTagsList,
                                                                  withTagsList,
                                                                  QString(),
                                                                  0,
                                                                  createdTime,
                                                                  updatedTime,
                                                                  shares,
                                                                  hidden,
                                                                  QString(),
                                                                  false,
                                                                  -1,
                                                                  -1,
                                                                  mediaStringList,
                                                                  isVideo,
                                                                  this);
        eventMap.insert(ContentItemRole, QVariant::fromValue(post));
        eventMap.insert(TimestampRole, QDateTime::fromString(createdTime, Qt::ISODate));
        data.append(eventMap);
    }

    updateData(data);
}
