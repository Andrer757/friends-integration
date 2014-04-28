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

#ifndef FRIENDSPOSTINTERFACE_H
#define FRIENDSPOSTINTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtQml/QQmlListProperty>

class FriendsObjectReferenceInterface: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString objectIdentifier READ objectIdentifier CONSTANT)
    Q_PROPERTY(QString objectName READ objectName CONSTANT)
public:
    virtual ~FriendsObjectReferenceInterface();
    QString objectIdentifier() const;
    QString objectName() const;
    static FriendsObjectReferenceInterface * create(const QString &objectIdentifier,
                                                    const QString &objectName,
                                                    QObject *parent = 0);
private:
    explicit FriendsObjectReferenceInterface(QObject *parent = 0);
    QString m_objectIdentifier;
    QString m_objectName;
};

class FriendsNameTagInterface: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userIdentifier READ userIdentifier CONSTANT)
    Q_PROPERTY(QString userName READ userName CONSTANT)
    Q_PROPERTY(QString nameTagType READ nameTagType CONSTANT)
    Q_PROPERTY(int offset READ offset CONSTANT)
    Q_PROPERTY(int length READ length CONSTANT)
public:
    QString userIdentifier() const;
    QString userName() const;
    QString nameTagType() const;
    int offset() const;
    int length() const;
    virtual ~FriendsNameTagInterface();
    static FriendsNameTagInterface * create(const QString &userIdentifier,
                                            const QString &userName,
                                            const QString &nameTagType,
                                            int offset,
                                            int length,
                                            QObject *parent = 0);
private:
    QString m_userIdentifier;
    QString m_userName;
    QString m_nameTagType;
    int m_offset;
    int m_length;
    explicit FriendsNameTagInterface(QObject *parent = 0);
};

class FriendsPostInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FriendsObjectReferenceInterface * from READ from CONSTANT)
    Q_PROPERTY(QQmlListProperty<FriendsObjectReferenceInterface> to READ to CONSTANT)
    Q_PROPERTY(QString message READ message CONSTANT)
    Q_PROPERTY(QQmlListProperty<FriendsNameTagInterface> messageTags READ messageTags CONSTANT)
    Q_PROPERTY(QUrl picture READ picture CONSTANT)
    Q_PROPERTY(QUrl link READ link CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString caption READ caption CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(QUrl source READ source CONSTANT)
    Q_PROPERTY(QString postType READ postType CONSTANT)
    Q_PROPERTY(QString story READ story CONSTANT)
    Q_PROPERTY(QQmlListProperty<FriendsNameTagInterface> storyTags READ storyTags CONSTANT)
    Q_PROPERTY(QQmlListProperty<FriendsObjectReferenceInterface> withTags READ withTags CONSTANT)
    Q_PROPERTY(QString objectIdentifier READ objectIdentifier CONSTANT)
    Q_PROPERTY(FriendsObjectReferenceInterface * application READ application CONSTANT)
    Q_PROPERTY(QString createdTime READ createdTime CONSTANT)
    Q_PROPERTY(QString updatedTime READ updatedTime CONSTANT)
    Q_PROPERTY(int shares READ shares CONSTANT)
    Q_PROPERTY(bool hidden READ hidden CONSTANT)
    Q_PROPERTY(QString statusType READ statusType CONSTANT)
    Q_PROPERTY(bool liked READ liked CONSTANT)
    Q_PROPERTY(int likesCount READ likesCount CONSTANT)
    Q_PROPERTY(int commentsCount READ commentsCount CONSTANT)
    Q_PROPERTY(QStringList media READ media CONSTANT)
    Q_PROPERTY(bool isVideo READ isVideo CONSTANT)
public:
    virtual ~FriendsPostInterface();
    FriendsObjectReferenceInterface * from() const;
    QQmlListProperty<FriendsObjectReferenceInterface> to();
    QString message() const;
    QQmlListProperty<FriendsNameTagInterface> messageTags();
    QUrl picture() const;
    QUrl link() const;
    QString name() const;
    QString caption() const;
    QString description() const;
    QUrl source() const;
    QString postType() const;
    QString story() const;
    QQmlListProperty<FriendsNameTagInterface> storyTags();
    QQmlListProperty<FriendsObjectReferenceInterface> withTags();
    QString objectIdentifier() const;
    FriendsObjectReferenceInterface * application() const;
    QString createdTime() const;
    QString updatedTime() const;
    int shares() const;
    bool hidden() const;
    QString statusType() const;
    bool liked() const;
    int likesCount() const;
    int commentsCount() const;
    QStringList media() const;
    bool isVideo() const;
    static FriendsPostInterface * create(FriendsObjectReferenceInterface *from,
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
                                         QObject *parent = 0);
signals:
    void loaded();
private:
    explicit FriendsPostInterface(QObject *parent = 0);
    static FriendsObjectReferenceInterface * to_at(QQmlListProperty<FriendsObjectReferenceInterface> *list, int index);
    static int to_count(QQmlListProperty<FriendsObjectReferenceInterface> *list);
    static FriendsNameTagInterface * messageTags_at(QQmlListProperty<FriendsNameTagInterface> *list, int index);
    static int messageTags_count(QQmlListProperty<FriendsNameTagInterface> *list);
    static FriendsNameTagInterface * storyTags_at(QQmlListProperty<FriendsNameTagInterface> *list, int index);
    static int storyTags_count(QQmlListProperty<FriendsNameTagInterface> *list);
    static FriendsObjectReferenceInterface * withTags_at(QQmlListProperty<FriendsObjectReferenceInterface> *list, int index);
    static int withTags_count(QQmlListProperty<FriendsObjectReferenceInterface> *list);
    FriendsObjectReferenceInterface *m_from;
    QList<FriendsObjectReferenceInterface *> m_to;
    QString m_message;
    QList<FriendsNameTagInterface *> m_messageTags;
    QUrl m_picture;
    QUrl m_link;
    QString m_name;
    QString m_caption;
    QString m_description;
    QUrl m_source;
    QString m_postType;
    QString m_story;
    QList<FriendsNameTagInterface *> m_storyTags;
    QList<FriendsObjectReferenceInterface *> m_withTags;
    QString m_objectIdentifier;
    FriendsObjectReferenceInterface *m_application;
    QString m_createdTime;
    QString m_updatedTime;
    int m_shares;
    bool m_hidden;
    QString m_statusType;
    bool m_liked;
    int m_likesCount;
    int m_commentsCount;
    QStringList m_media;
    bool m_isVideo;
};

#endif // FRIENDSPOSTINTERFACE_H
