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

FriendsIntegrationClient::FriendsIntegrationClient(const QString& pluginName,
                                 const Buteo::SyncProfile& profile,
                                 Buteo::PluginCbInterface *cbInterface)
    : ClientPlugin(pluginName, profile, cbInterface)
    , m_network(new QNetworkAccessManager(this))
    , m_notificationsReply(0)
{
    QSettings settings (ORGANIZATION_NAME, APPLICATION_NAME);
    m_token = settings.value("login/token").toString();
}

FriendsIntegrationClient::~FriendsIntegrationClient()
{
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
    QUrl notificationsUrl ("https://graph.facebook.com/me/notifications");
    QUrlQuery notificationsUrlQuery (notificationsUrl);
    notificationsUrlQuery.addQueryItem("include_read", "1");
    notificationsUrlQuery.addQueryItem("fields", "id,created_time,unread");
    notificationsUrlQuery.addQueryItem("limit", "50");
    notificationsUrlQuery.addQueryItem("access_token", m_token);
    notificationsUrl.setQuery(notificationsUrlQuery);
    m_notificationsReply = m_network->get(QNetworkRequest(notificationsUrl));

    connect(m_notificationsReply, &QNetworkReply::finished,
            this, &FriendsIntegrationClient::slotNotificationsFinished);

    return true;
}

void FriendsIntegrationClient::abortSync(Sync::SyncStatus status)
{
    Q_UNUSED(status)
    if (m_notificationsReply) {
        m_notificationsReply->deleteLater();
        m_notificationsReply = 0;
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

    return true;
}

void FriendsIntegrationClient::slotNotificationsFinished()
{
    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(m_notificationsReply->readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        emit error(getProfileName(), "Sync failed", Buteo::SyncResults::SYNC_RESULT_FAILED);
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
        emit success(getProfileName(), "Sync succeded");
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

    emit success(getProfileName(), "Sync succeded");
}

void FriendsIntegrationClient::connectivityStateChanged(Sync::ConnectivityType type, bool state)
{
    Q_UNUSED(type)
    Q_UNUSED(state)
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
