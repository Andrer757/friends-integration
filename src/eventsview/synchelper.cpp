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

#include "synchelper.h"
#include <buteosyncfw5/SyncCommonDefs.h>

static const char *FRIENDS_INTEGRATION_SYNC = "friendsintegration";

SyncHelper::SyncHelper(QObject *parent) :
    QObject(parent), QQmlParserStatus()
    , m_complete(false), m_loading(false)
{
    m_interface = new Buteo::SyncClientInterface();
    connect(m_interface, &Buteo::SyncClientInterface::syncStatus,
            this, &SyncHelper::slotSyncStatus);
}

void SyncHelper::classBegin()
{
}

void SyncHelper::componentComplete()
{
    m_complete = true;
    checkCurrentRun();
}

bool SyncHelper::loading() const
{
    return m_loading;
}

void SyncHelper::setLoading(bool loading)
{
    if (m_loading != loading) {
        m_loading = loading;
        emit loadingChanged();
    }
}

void SyncHelper::sync()
{
    m_interface->startSync(FRIENDS_INTEGRATION_SYNC);
}

void SyncHelper::slotSyncStatus(const QString &aProfileId, int aStatus,
                                const QString &aMessage, int aStatusDetails)
{
    Q_UNUSED(aMessage)
    Q_UNUSED(aStatusDetails)

    if (!aProfileId.startsWith(FRIENDS_INTEGRATION_SYNC)) {
        return;
    }

    bool newLoading = (aStatus == Sync::SYNC_QUEUED || aStatus == Sync::SYNC_STARTED
                       || aStatus == Sync::SYNC_PROGRESS);

    if (!newLoading) {
        m_activeSyncs.removeAll(aProfileId);
        setLoading(m_activeSyncs.count() > 0);
    } else if (!m_activeSyncs.contains(aProfileId)) {
        m_activeSyncs.append(aProfileId);
        setLoading(true);
    }
}

void SyncHelper::checkCurrentRun()
{
    // Get the current running syncs to see if we are running
    QStringList runningSyncs = m_interface->getRunningSyncList();
    foreach (const QString profile, runningSyncs) {
        slotSyncStatus(profile, Sync::SYNC_PROGRESS, "", 0);
    }
}
