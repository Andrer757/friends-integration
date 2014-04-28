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

#include "friendspostinterface.h"

FriendsObjectReferenceInterface::FriendsObjectReferenceInterface(QObject *parent)
    : QObject(parent)
{
}

FriendsObjectReferenceInterface::~FriendsObjectReferenceInterface()
{
}

QString FriendsObjectReferenceInterface::objectIdentifier() const
{
    return m_objectIdentifier;
}

QString FriendsObjectReferenceInterface::objectName() const
{
    return m_objectName;
}

FriendsObjectReferenceInterface * FriendsObjectReferenceInterface::create(const QString &objectIdentifier,
                                                                          const QString &objectName,
                                                                          QObject *parent)
{
    FriendsObjectReferenceInterface *object = new FriendsObjectReferenceInterface(parent);
    object->m_objectIdentifier = objectIdentifier;
    object->m_objectName = objectName;
    return object;
}

FriendsNameTagInterface::FriendsNameTagInterface(QObject *parent)
    : QObject(parent)
{
}

FriendsNameTagInterface::~FriendsNameTagInterface()
{
}

FriendsNameTagInterface * FriendsNameTagInterface::create(const QString &userIdentifier,
                                                          const QString &userName,
                                                          const QString &nameTagType,
                                                          int offset, int length, QObject *parent)
{
    FriendsNameTagInterface *object = new FriendsNameTagInterface(parent);
    object->m_userIdentifier = userIdentifier;
    object->m_userName = userName;
    object->m_nameTagType = nameTagType;
    object->m_offset = offset;
    object->m_length = length;

    return object;
}

QString FriendsNameTagInterface::userIdentifier() const
{
    return m_userIdentifier;
}

QString FriendsNameTagInterface::userName() const
{
    return m_userName;
}

QString FriendsNameTagInterface::nameTagType() const
{
    return m_nameTagType;
}

int FriendsNameTagInterface::offset() const
{
    return m_offset;
}

int FriendsNameTagInterface::length() const
{
    return m_length;
}

FriendsPostInterface::FriendsPostInterface(QObject *parent)
    : QObject(parent), m_from(0), m_application(0), m_shares(-1), m_hidden(false), m_liked(false)
    , m_likesCount(-1), m_commentsCount(-1)
{
}

FriendsPostInterface::~FriendsPostInterface()
{
}

FriendsObjectReferenceInterface * FriendsPostInterface::from() const
{
    return m_from;
}

QQmlListProperty<FriendsObjectReferenceInterface> FriendsPostInterface::to()
{
    return QQmlListProperty<FriendsObjectReferenceInterface>(
                    this, 0,
                    &FriendsPostInterface::to_count,
                    &FriendsPostInterface::to_at);
}

QString FriendsPostInterface::message() const
{
    return m_message;
}

QQmlListProperty<FriendsNameTagInterface> FriendsPostInterface::messageTags()
{
    return QQmlListProperty<FriendsNameTagInterface>(
                    this, 0,
                    &FriendsPostInterface::messageTags_count,
                    &FriendsPostInterface::messageTags_at);
}

QUrl FriendsPostInterface::picture() const
{
    return m_picture;
}

QUrl FriendsPostInterface::link() const
{
    return m_link;
}

QString FriendsPostInterface::name() const
{
    return m_name;
}

QString FriendsPostInterface::caption() const
{
    return m_caption;
}

QString FriendsPostInterface::description() const
{
    return m_description;
}

QUrl FriendsPostInterface::source() const
{
    return m_source;
}

QString FriendsPostInterface::postType() const
{
    return m_postType;
}

QString FriendsPostInterface::story() const
{
    return m_story;
}

QQmlListProperty<FriendsNameTagInterface> FriendsPostInterface::storyTags()
{
    return QQmlListProperty<FriendsNameTagInterface>(
                    this, 0,
                    &FriendsPostInterface::storyTags_count,
                    &FriendsPostInterface::storyTags_at);
}

QQmlListProperty<FriendsObjectReferenceInterface> FriendsPostInterface::withTags()
{
    return QQmlListProperty<FriendsObjectReferenceInterface>(
                    this, 0,
                    &FriendsPostInterface::withTags_count,
                    &FriendsPostInterface::withTags_at);
}

QString FriendsPostInterface::objectIdentifier() const
{
    return m_objectIdentifier;
}

FriendsObjectReferenceInterface * FriendsPostInterface::application() const
{
    return m_application;
}

QString FriendsPostInterface::createdTime() const
{
    return m_createdTime;
}

QString FriendsPostInterface::updatedTime() const
{
    return m_updatedTime;
}

int FriendsPostInterface::shares() const
{
    return m_shares;
}

bool FriendsPostInterface::hidden() const
{
    return m_hidden;
}

QString FriendsPostInterface::statusType() const
{
    return m_statusType;
}

bool FriendsPostInterface::liked() const
{
    return m_liked;
}

int FriendsPostInterface::likesCount() const
{
    return m_likesCount;
}

int FriendsPostInterface::commentsCount() const
{
    return m_commentsCount;
}

QStringList FriendsPostInterface::media() const
{
    return m_media;
}

bool FriendsPostInterface::isVideo() const
{
    return m_isVideo;
}

FriendsPostInterface * FriendsPostInterface::create(FriendsObjectReferenceInterface *from,
                                                               const QList<FriendsObjectReferenceInterface *> &to,
                                                               const QString &message,
                                                               const QList<FriendsNameTagInterface *> &messageTags,
                                                               const QUrl &picture,
                                                               const QUrl &link,
                                                               const QString &name,
                                                               const QString &caption,
                                                               const QString &description,
                                                               const QUrl &source,
                                                               const QString &postType,
                                                               const QString &story,
                                                               const QList<FriendsNameTagInterface *> &storyTags,
                                                               const QList<FriendsObjectReferenceInterface *> &withTags,
                                                               const QString &objectIdentifier,
                                                               FriendsObjectReferenceInterface *application,
                                                               const QString &createdTime,
                                                               const QString &updatedTime,
                                                               int shares,
                                                               bool hidden,
                                                               const QString &statusType,
                                                               bool liked,
                                                               int likesCount,
                                                               int commentsCount,
                                                               const QStringList &media,
                                                               bool isVideo,
                                                               QObject *parent)
{
    FriendsPostInterface *object = new FriendsPostInterface(parent);
    object->m_from = from;
    object->m_to = to;
    object->m_message = message;
    object->m_messageTags = messageTags;
    object->m_picture = picture;
    object->m_link = link;
    object->m_name = name;
    object->m_caption = caption;
    object->m_description = description;
    object->m_source = source;
    object->m_postType = postType;
    object->m_story = story;
    object->m_storyTags = storyTags;
    object->m_withTags = withTags;
    object->m_objectIdentifier = objectIdentifier;
    object->m_application = application;
    object->m_createdTime = createdTime;
    object->m_updatedTime = updatedTime;
    object->m_shares = shares;
    object->m_hidden = hidden;
    object->m_statusType = statusType;
    object->m_liked = liked;
    object->m_likesCount = likesCount;
    object->m_commentsCount = commentsCount;
    object->m_media = media;
    object->m_isVideo = isVideo;
    return object;
}

FriendsObjectReferenceInterface * FriendsPostInterface::to_at(QQmlListProperty<FriendsObjectReferenceInterface> *list, int index)
{
    FriendsPostInterface *interface = qobject_cast<FriendsPostInterface *>(list->object);
    if (interface
        && index < interface->m_to.count()
        && index >= 0) {
        return interface->m_to.at(index);
    }
    return 0;
}
int FriendsPostInterface::to_count(QQmlListProperty<FriendsObjectReferenceInterface> *list)
{
    FriendsPostInterface *interface = qobject_cast<FriendsPostInterface *>(list->object);
    if (interface) {
        return interface->m_to.count();
    }
    return 0;
}

FriendsNameTagInterface * FriendsPostInterface::messageTags_at(QQmlListProperty<FriendsNameTagInterface> *list, int index)
{
    FriendsPostInterface *interface = qobject_cast<FriendsPostInterface *>(list->object);
    if (interface
        && index < interface->m_messageTags.count()
        && index >= 0) {
        return interface->m_messageTags.at(index);
    }
    return 0;
}
int FriendsPostInterface::messageTags_count(QQmlListProperty<FriendsNameTagInterface> *list)
{
    FriendsPostInterface *interface = qobject_cast<FriendsPostInterface *>(list->object);
    if (interface) {
        return interface->m_messageTags.count();
    }
    return 0;
}

FriendsNameTagInterface * FriendsPostInterface::storyTags_at(QQmlListProperty<FriendsNameTagInterface> *list, int index)
{
    FriendsPostInterface *interface = qobject_cast<FriendsPostInterface *>(list->object);
    if (interface
        && index < interface->m_storyTags.count()
        && index >= 0) {
        return interface->m_storyTags.at(index);
    }
    return 0;
}
int FriendsPostInterface::storyTags_count(QQmlListProperty<FriendsNameTagInterface> *list)
{
    FriendsPostInterface *interface = qobject_cast<FriendsPostInterface *>(list->object);
    if (interface) {
        return interface->m_storyTags.count();
    }
    return 0;
}

FriendsObjectReferenceInterface * FriendsPostInterface::withTags_at(QQmlListProperty<FriendsObjectReferenceInterface> *list, int index)
{
    FriendsPostInterface *interface = qobject_cast<FriendsPostInterface *>(list->object);
    if (interface
        && index < interface->m_withTags.count()
        && index >= 0) {
        return interface->m_withTags.at(index);
    }
    return 0;
}
int FriendsPostInterface::withTags_count(QQmlListProperty<FriendsObjectReferenceInterface> *list)
{
    FriendsPostInterface *interface = qobject_cast<FriendsPostInterface *>(list->object);
    if (interface) {
        return interface->m_withTags.count();
    }
    return 0;
}
