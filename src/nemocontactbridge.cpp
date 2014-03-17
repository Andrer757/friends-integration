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

#include "nemocontactbridge.h"
#include <QtCore/QDebug>
#include <QtCore/QThread>
#include <QtContacts/QContact>
#include <QtContacts/QContactManager>
#include <QtContacts/QContactGuid>
#include <QtContacts/QContactDetail>
#include <QtContacts/QContactNickname>

QTCONTACTS_USE_NAMESPACE

static const char *ID_KEY = "qtcontacts:org.nemomobile.contacts.sqlite::sql-%1";

class NemoContactBridgeWorkerObject: public QObject
{
    Q_OBJECT
public:
    explicit NemoContactBridgeWorkerObject(QObject *parent = 0);
public Q_SLOTS:
    void setContactId(const QString &contactId);
    void run();
Q_SIGNALS:
    void done(const QString &facebookId = QString());
private:
    QContactManager *m_manager;
    QString m_contactId;
};

NemoContactBridgeWorkerObject::NemoContactBridgeWorkerObject(QObject *parent)
    : QObject(parent)
{
    m_manager = new QContactManager(this);
}

void NemoContactBridgeWorkerObject::setContactId(const QString &contactId)
{
    m_contactId = contactId;
}

void NemoContactBridgeWorkerObject::run()
{
    QString idStr = QString(ID_KEY).arg(m_contactId);
    QContactId id = QContactId::fromString(idStr);
    qDebug() << "Using id:" << idStr;
    QContact contact = m_manager->contact(id);

    if (contact.isEmpty()) {
        emit done();
        return;
    }

    QContactNickname nickname = contact.detail<QContactNickname>();
    if (nickname.isEmpty()) {
        emit done();
        return;
    }

    QMap<int, QVariant> nicknameValues = nickname.values();
    nicknameValues.remove((int) QContactNickname::FieldNickname);
    if (nicknameValues.count() != 1) {
        emit done();
        return;
    }
    QStringList facebookKeys = nicknameValues.values().first().toString().split(":");
    if (facebookKeys.isEmpty()) {
        emit done();
        return;
    }

    id = QContactId::fromString(QString(ID_KEY).arg(facebookKeys.first()));
    QContact fbContact = m_manager->contact(id);

    emit done(fbContact.detail<QContactGuid>().guid());
}


class NemoContactBridgePrivate: public QObject
{
    Q_OBJECT
public:
    explicit NemoContactBridgePrivate(NemoContactBridge *q, QObject *parent = 0);
    void fetch();
    QString contactId;
    QString facebookId;
    QContact contact;
    QThread *thread;
    NemoContactBridgeWorkerObject *object;
public Q_SLOTS:
    void done(const QString &facebookId);
Q_SIGNALS:
    void setContactId(const QString &contactId);
    void run();
protected:
    NemoContactBridge * const q_ptr;
private:
    Q_DECLARE_PUBLIC(NemoContactBridge)
};

NemoContactBridgePrivate::NemoContactBridgePrivate(NemoContactBridge *q, QObject *parent)
    : QObject(parent), thread(0), object(0), q_ptr(q)
{
}

void NemoContactBridgePrivate::fetch()
{
    emit setContactId(contactId);
    emit run();
}

void NemoContactBridgePrivate::done(const QString &facebookId)
{
    Q_Q(NemoContactBridge);
    if (this->facebookId != facebookId) {
        this->facebookId = facebookId;
        emit q->facebookIdChanged();
    }
}

NemoContactBridge::NemoContactBridge(QObject *parent) :
    QObject(parent), d_ptr(new NemoContactBridgePrivate(this))
{
    Q_D(NemoContactBridge);
    d->thread = new QThread(this);
    d->object = new NemoContactBridgeWorkerObject();
    connect(d, &NemoContactBridgePrivate::setContactId,
            d->object, &NemoContactBridgeWorkerObject::setContactId);
    connect(d, &NemoContactBridgePrivate::run, d->object, &NemoContactBridgeWorkerObject::run);
    connect(d->object, &NemoContactBridgeWorkerObject::done, d, &NemoContactBridgePrivate::done);
    d->object->moveToThread(d->thread);
    d->thread->start();
}

NemoContactBridge::~NemoContactBridge()
{
    Q_D(NemoContactBridge);
    d->thread->quit();
    d->thread->wait();
    d->deleteLater();
}

QString NemoContactBridge::contactId() const
{
    Q_D(const NemoContactBridge);
    return d->contactId;
}

void NemoContactBridge::setContactId(const QString &contactId)
{
    Q_D(NemoContactBridge);
    if (d->contactId != contactId) {
        d->contactId = contactId;
        emit contactIdChanged();
    }
}

QString NemoContactBridge::facebookId() const
{
    Q_D(const NemoContactBridge);
    return d->facebookId;
}

void NemoContactBridge::fetch()
{
    Q_D(NemoContactBridge);
    d->fetch();
}

#include "nemocontactbridge.moc"
