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

#ifndef FRIENDSINTEGRATIONCLIENT_H
#define FRIENDSINTEGRATIONCLIENT_H

#include <ClientPlugin.h>
#include <SyncResults.h>

class QNetworkAccessManager;
class QNetworkReply;
class FriendsIntegrationClient : public Buteo::ClientPlugin
{
    Q_OBJECT

public:
    explicit FriendsIntegrationClient(const QString& pluginName, const Buteo::SyncProfile& profile,
                                      Buteo::PluginCbInterface *cbInterface);
    virtual ~FriendsIntegrationClient();
    bool init();
    bool uninit();
    bool startSync();
    void abortSync(Sync::SyncStatus status = Sync::SYNC_ABORTED);
    Buteo::SyncResults getSyncResults() const;
    bool cleanUp();
public slots:
    void connectivityStateChanged(Sync::ConnectivityType type, bool state);
private:
    Buteo::SyncResults m_results;
    QString m_token;
    QNetworkAccessManager *m_network;
    QNetworkReply *m_notificationsReply;
private slots:
    void slotNotificationsFinished();
};

extern "C" FriendsIntegrationClient* createPlugin(const QString& pluginName,
                                                  const Buteo::SyncProfile& profile,
                                                  Buteo::PluginCbInterface *cbInterface);
extern "C" void destroyPlugin(FriendsIntegrationClient *client);

#endif // FRIENDSINTEGRATIONCLIENT_H
