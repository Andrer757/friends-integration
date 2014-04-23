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

#include "friendsjsonpostdatabase.h"
#include "abstractsocialcachedatabase_p.h"
#include "socialsyncinterface.h"
#include <QtCore/QDebug>
#include <QtSql/QSqlError>

static const int POST_DB_VERSION = 1;
static const char *SERVICE_NAME = "friends";
static const char *DB_FILE = "friends.db";

struct PostData
{
    QString identifier;
    QJsonDocument post;
    QDateTime timestamp;
};

class FriendsJsonPostDatabasePrivate: public AbstractSocialCacheDatabasePrivate
{
public:
    FriendsJsonPostDatabasePrivate(AbstractSocialCacheDatabase *q, const QString &serviceName,
                                   const QString &databaseFile);
    ~FriendsJsonPostDatabasePrivate();
private:
    struct {
        QMap<QString, PostData> insertPosts;
        bool remove;
    } queue;

    QList<QJsonDocument> asyncPosts;
    QList<QJsonDocument> posts;
    Q_DECLARE_PUBLIC(FriendsJsonPostDatabase)
};

FriendsJsonPostDatabasePrivate::FriendsJsonPostDatabasePrivate(AbstractSocialCacheDatabase *q,
                                                               const QString &serviceName,
                                                               const QString &databaseFile)
    : AbstractSocialCacheDatabasePrivate(q, serviceName,
                                         SocialSyncInterface::dataType(SocialSyncInterface::Posts),
                                         databaseFile, POST_DB_VERSION)
{
}

FriendsJsonPostDatabasePrivate::~FriendsJsonPostDatabasePrivate()
{
    Q_Q(FriendsJsonPostDatabase);
    q->cancelRead();
    q->wait();
}

FriendsJsonPostDatabase::FriendsJsonPostDatabase()
    : AbstractSocialCacheDatabase(*(new FriendsJsonPostDatabasePrivate(this, SERVICE_NAME, DB_FILE)))
{
}

FriendsJsonPostDatabase::~FriendsJsonPostDatabase()
{
}

QList<QJsonDocument> FriendsJsonPostDatabase::posts() const
{
    return d_func()->posts;
}

void FriendsJsonPostDatabase::addPost(const QString &identifier, const QJsonDocument &post,
                                      const QDateTime &timestamp)
{
    Q_D(FriendsJsonPostDatabase);
    QMutexLocker locker(&d->mutex);

    PostData data;
    data.identifier = identifier;
    data.post = post;
    data.timestamp = timestamp;
    d->queue.insertPosts.insert(identifier, data);
}

void FriendsJsonPostDatabase::removePosts()
{
    Q_D(FriendsJsonPostDatabase);

    QMutexLocker locker(&d->mutex);
    d->queue.remove = true;
}

void FriendsJsonPostDatabase::commit()
{
    executeWrite();
}

void FriendsJsonPostDatabase::refresh()
{
    executeRead();
}

bool FriendsJsonPostDatabase::read()
{
    Q_D(FriendsJsonPostDatabase);
    // This might be slow

    QSqlQuery postQuery = prepare(QLatin1String(
                "SELECT post "
                "FROM posts "
                "ORDER BY timestamp DESC"));

    if (!postQuery.exec()) {
        qWarning() << Q_FUNC_INFO << "Error reading from posts table:" << postQuery.lastError();
        return false;
    }

    QList<QJsonDocument> posts;
    while (postQuery.next()) {
        QByteArray jsonString = postQuery.value(0).toByteArray();
        QJsonParseError error;
        QJsonDocument post = QJsonDocument::fromJson(jsonString, &error);
        if (error.error == QJsonParseError::NoError) {
            posts.append(post);
        }
    }

    QMutexLocker locker(&d->mutex);
    d->asyncPosts = posts;

    return true;
}

bool FriendsJsonPostDatabase::write()
{
    Q_D(FriendsJsonPostDatabase);

    QMutexLocker locker(&d->mutex);

    const QMap<QString, PostData> insertPosts = d->queue.insertPosts;
    bool remove = d->queue.remove;

    d->queue.insertPosts.clear();
    d->queue.remove = false;

    locker.unlock();

    bool success = true;

    QSqlQuery query;

    // perform removals first.
    if (!remove) {
        query = prepare(QStringLiteral("DELETE FROM posts"));
        executeSocialCacheQuery(query);
    }

    struct {
        QVariantList postIds;
        QVariantList posts;
        QVariantList timestamps;
    } posts;


    Q_FOREACH (const PostData &post, insertPosts) {
        const QVariant postId = post.identifier;

        posts.postIds.append(postId);
        posts.posts.append(post.post.toJson(QJsonDocument::Compact));
        posts.timestamps.append(post.timestamp.toTime_t());
    }

    if (!posts.postIds.isEmpty()) {
        query = prepare(QStringLiteral(
                    "INSERT OR REPLACE INTO posts ("
                    " identifier, post, timestamp) "
                    "VALUES ("
                    " :postId, :post, :timestamp)"));
        query.bindValue(QStringLiteral(":postId"), posts.postIds);
        query.bindValue(QStringLiteral(":post"), posts.posts);
        query.bindValue(QStringLiteral(":timestamp"), posts.timestamps);
        executeBatchSocialCacheQuery(query);
    }

    return success;
}

bool FriendsJsonPostDatabase::createTables(QSqlDatabase database) const
{
    QSqlQuery query (database);
    query.prepare( "CREATE TABLE IF NOT EXISTS posts ("\
                   "identifier TEXT UNIQUE PRIMARY KEY,"\
                   "post TEXT,"\
                   "timestamp INTEGER)");
    if (!query.exec()) {
        qWarning() << Q_FUNC_INFO << "Unable to create posts table" << query.lastError().text();
        return false;
    }
    return true;
}

bool FriendsJsonPostDatabase::dropTables(QSqlDatabase database) const
{
    QSqlQuery query(database);
    query.prepare("DROP TABLE IF EXISTS posts");
    if (!query.exec()) {
        qWarning() << Q_FUNC_INFO << "Unable to delete posts table"
                   << query.lastError().text();
        return false;
    }

    return true;
}

void FriendsJsonPostDatabase::readFinished()
{
    Q_D(FriendsJsonPostDatabase);
    QMutexLocker locker(&d->mutex);

    d->posts = d->asyncPosts;
    d->asyncPosts.clear();

    locker.unlock();

    emit postsChanged();
}


