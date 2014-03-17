/*
 * Copyright (C) 2013 Jolla Ltd. <chris.adams@jollamobile.com>
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
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
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

#include <QtGlobal>
#include <QtQml>
#include <QQmlEngine>
#include <QQmlExtensionPlugin>
#include "nemocontactbridge.h"
#include "friendsinvoker.h"

// social plugin headers
#include "socialnetworkinterface.h"
#include "socialnetworkmodelinterface.h"
#include "contentiteminterface.h"
#include "identifiablecontentiteminterface.h"
#include "filterinterface.h"

// facebook implementation headers
#include "facebook/facebookinterface.h"
#include "facebook/facebookitemfilterinterface.h"
#include "facebook/facebookrelateddatafilterinterface.h"
#include "facebook/facebookobjectreferenceinterface.h"
#include "facebook/facebookalbuminterface.h"
#include "facebook/facebookcommentinterface.h"
#include "facebook/facebookeventinterface.h"
#include "facebook/facebookgroupinterface.h"
#include "facebook/facebooknotificationinterface.h"
#include "facebook/facebookpageinterface.h"
#include "facebook/facebookphotointerface.h"
#include "facebook/facebookpostinterface.h"
#include "facebook/facebookuserinterface.h"
#include "facebook/facebooklikeinterface.h"

// Social extra features
#include "socialextra/alphabeticalsorterinterface.h"
#include "socialextra/newsfeedfilterinterface.h"
#include "socialextra/facebookextrapostinterface.h"
#include "socialextra/typesolverinterface.h"
#include "socialextra/filterablefacebookrelateddatafilterinterface.h"
#include "socialextra/eventfilterinterface.h"
#include "socialextra/facebookextraeventinterface.h"
#include "socialextra/facebookextrainterface.h"
#include "socialextra/commentfilterinterface.h"

// Friends plugin
#include "tokenmanager.h"
#include "posthelper.h"
#include "footerhelper.h"
#include "notificationshelper.h"
#include "imagehelper.h"
#include "imagemanager.h"
#include "userinfohelper.h"
#include "datehelper.h"

#include <QGuiApplication>

// using custom translator so it gets properly removed from qApp when engine is deleted
class AppTranslator: public QTranslator
{
    Q_OBJECT
public:
    AppTranslator(QObject *parent)
        : QTranslator(parent)
    {
        qApp->installTranslator(this);
    }

    virtual ~AppTranslator()
    {
        qApp->removeTranslator(this);
    }
};

static const char *REASON = "Cannot be created";

static QObject *imagemanager_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return new ImageManager();
}

static QObject * datehelper_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return new DateHelper();
}

static QObject * footerhelper_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return new FooterHelper();
}

static QObject * notificationshelper_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return new NotificationsHelper();
}


class Q_DECL_EXPORT FriendsIntegrationPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.SfietKonstantin.friends.integration")
public:
    virtual ~FriendsIntegrationPlugin() { }

    void initializeEngine(QQmlEngine *engine, const char *uri)
    {
        Q_ASSERT(uri == QLatin1String("org.SfietKonstantin.friends.integration"));
        Q_UNUSED(uri)
        AppTranslator *engineeringEnglish = new AppTranslator(engine);
        AppTranslator *translator = new AppTranslator(engine);
        engineeringEnglish->load("friends-engineering-english", "/usr/share/harbour-friends/translations/");
        translator->load(QLocale(), "friends", "_", "/usr/share/harbour-friends/translations/");
    }

    void registerTypes(const char *uri)
    {

        Q_ASSERT(uri == QLatin1String("org.SfietKonstantin.friends.integration"));
        qmlRegisterType<NemoContactBridge>(uri, 1, 0, "NemoContactBridge");
        qmlRegisterType<FriendsInvoker>(uri, 1, 0, "FriendsInvoker");

        // Social
        qmlRegisterUncreatableType<SocialNetworkInterface>(uri, 1, 0, "SocialNetwork", REASON);
        qmlRegisterUncreatableType<ContentItemInterface>(uri, 1, 0, "ContentItem", REASON);
        qmlRegisterUncreatableType<IdentifiableContentItemInterface>(uri, 1, 0, "IdentifiableContentItem", REASON);
        qmlRegisterUncreatableType<FilterInterface>(uri, 1, 0, "Filter", REASON);
        qmlRegisterUncreatableType<SorterInterface>(uri, 1, 0, "Sorter", REASON);

        // creatable types from the social plugin
        qmlRegisterType<SocialNetworkModelInterface>(uri, 1, 0, "SocialNetworkModel");

        // creatable types from the facebook implementation
        qmlRegisterType<FacebookInterface>(uri, 1, 0, "Facebook");
        qmlRegisterType<FacebookItemFilterInterface>(uri, 1, 0, "FacebookItemFilter");
        qmlRegisterType<FacebookRelatedDataFilterInterface>(uri, 1, 0, "FacebookRelatedDataFilter");
        qmlRegisterType<FacebookObjectReferenceInterface>(uri, 1, 0, "FacebookObjectReference");
        qmlRegisterType<FacebookAlbumInterface>(uri, 1, 0, "FacebookAlbum");
        qmlRegisterType<FacebookCommentInterface>(uri, 1, 0, "FacebookComment");
        qmlRegisterType<FacebookEventInterface>(uri, 1, 0, "FacebookEvent");
        qmlRegisterType<FacebookGroupInterface>(uri, 1, 0, "FacebookGroup");
        qmlRegisterType<FacebookNotificationInterface>(uri, 1, 0, "FacebookNotification");
        qmlRegisterType<FacebookPageInterface>(uri, 1, 0, "FacebookPage");
        qmlRegisterType<FacebookPhotoInterface>(uri, 1, 0, "FacebookPhoto");
        qmlRegisterType<FacebookPostInterface>(uri, 1, 0, "FacebookPost");
        qmlRegisterType<FacebookUserInterface>(uri, 1, 0, "FacebookUser");

        qmlRegisterType<FacebookCoverInterface>(uri, 1, 0, "FacebookCover");
        qmlRegisterType<FacebookLikeInterface>(uri, 1, 0, "FacebookLike");
        qmlRegisterType<FacebookNameTagInterface>(uri, 1, 0, "FacebookNameTag");
        qmlRegisterType<FacebookPhotoImageInterface>(uri, 1, 0, "FacebookPhotoImage");
        qmlRegisterType<FacebookPhotoTagInterface>(uri, 1, 0, "FacebookPhotoTag");
        qmlRegisterType<FacebookPostActionInterface>(uri, 1, 0, "FacebookPostAction");
        qmlRegisterType<FacebookPostPropertyInterface>(uri, 1, 0, "FacebookPostProperty");
        qmlRegisterType<FacebookUserCoverInterface>(uri, 1, 0, "FacebookUserCover");
        qmlRegisterType<FacebookUserPictureInterface>(uri, 1, 0, "FacebookUserPicture");

        // Socialextra
        qmlRegisterType<AlphabeticalSorterInterface>(uri, 1, 0, "AlphabeticalSorter");
        qmlRegisterType<NewsFeedFilterInterface>(uri, 1, 0, "NewsFeedFilter");
        qmlRegisterType<FacebookExtraPostInterface>(uri, 1, 0, "FacebookExtraPost");
        qmlRegisterType<TypeSolverInterface>(uri, 1, 0, "TypeSolver");
        qmlRegisterType<TypeSolverFilterInterface>(uri, 1, 0, "TypeSolverFilter");
        qmlRegisterType<FilterableFacebookRelatedDataFilterInterface>(uri, 1, 0, "FilterableFacebookRelatedDataFilter");
        qmlRegisterType<EventFilterInterface>(uri, 1, 0, "EventFilter");
        qmlRegisterType<FacebookExtraEventInterface>(uri, 1, 0, "FacebookExtraEvent");
        qmlRegisterType<FacebookExtraInterface>(uri, 1, 0, "FacebookExtra");
        qmlRegisterType<CommentFilterInterface>(uri, 1, 0, "CommentFilter");

        // Friends
        qmlRegisterType<TokenManager>(uri, 1, 0, "TokenManager");
        qmlRegisterSingletonType<DateHelper>(uri, 1, 0, "DateHelper", datehelper_provider);
        qmlRegisterSingletonType<FooterHelper>(uri, 1, 0, "FooterHelper", footerhelper_provider);
        qmlRegisterSingletonType<NotificationsHelper>(uri, 1, 0, "NotificationsHelper",
                                               notificationshelper_provider);
        qmlRegisterType<PostHelper>(uri, 1, 0, "PostHelper");
        qmlRegisterType<ImageHelper>(uri, 1, 0, "ImageHelper");
        qmlRegisterSingletonType<ImageManager>(uri, 1, 0, "ImageManager", imagemanager_provider);
        qmlRegisterUncreatableType<QQuickImageBase>(uri, 1, 0, "QQuickImageBase", REASON);
        qmlRegisterType<UserInfoHelper>(uri, 1, 0, "UserInfoHelper");
    }
};

#include "plugin.moc"
