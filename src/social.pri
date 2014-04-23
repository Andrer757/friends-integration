equals(QT_MAJOR_VERSION, 5): DEFINES += QT_VERSION_5

system(sed -i \'s/import harbour.friends 1.0/import org.SfietKonstantin.friends.integration 1.0/g\' $$PWD/../friends/src/qml/*.qml)
system(sed -i \'s/import harbour.friends.social 1.0/import org.SfietKonstantin.friends.integration 1.0/g\' $$PWD/../friends/src/qml/*.qml)
system(sed -i \'s/import harbour.friends.social.extra 1.0/import org.SfietKonstantin.friends.integration 1.0/g\' $$PWD/../friends/src/qml/*.qml)

INCLUDEPATH += $$PWD/../social/src/

HEADERS += \
    $$PWD/../social/src/contentiteminterface.h \
    $$PWD/../social/src/contentiteminterface_p.h \
    $$PWD/../social/src/filterinterface.h \
    $$PWD/../social/src/filterinterface_p.h \
    $$PWD/../social/src/sorterinterface.h \
    $$PWD/../social/src/identifiablecontentiteminterface.h \
    $$PWD/../social/src/identifiablecontentiteminterface_p.h \
    $$PWD/../social/src/socialnetworkinterface.h \
    $$PWD/../social/src/socialnetworkinterface_p.h \
    $$PWD/../social/src/socialnetworkmodelinterface.h \
    $$PWD/../social/src/socialnetworkmodelinterface_p.h

SOURCES += \
    $$PWD/../social/src/contentiteminterface.cpp \
    $$PWD/../social/src/filterinterface.cpp \
    $$PWD/../social/src/sorterinterface.cpp \
    $$PWD/../social/src/identifiablecontentiteminterface.cpp \
    $$PWD/../social/src/socialnetworkinterface.cpp \
    $$PWD/../social/src/socialnetworkmodelinterface.cpp

# Facebook
HEADERS += \
    $$PWD/../social/src/facebook/facebookontology_p.h \
    $$PWD/../social/src/facebook/facebookinterface.h \
    $$PWD/../social/src/facebook/facebookinterface_p.h \
    $$PWD/../social/src/facebook/facebookobjectreferenceinterface.h \
    $$PWD/../social/src/facebook/facebookobjectreferenceinterface_p.h \
    $$PWD/../social/src/facebook/facebookalbuminterface.h \
    $$PWD/../social/src/facebook/facebookalbuminterface_p.h \
    $$PWD/../social/src/facebook/facebookcommentinterface.h \
    $$PWD/../social/src/facebook/facebookcommentinterface_p.h \
    $$PWD/../social/src/facebook/facebookeventinterface.h \
    $$PWD/../social/src/facebook/facebookeventinterface_p.h \
    $$PWD/../social/src/facebook/facebookgroupinterface.h \
    $$PWD/../social/src/facebook/facebookgroupinterface_p.h \
    $$PWD/../social/src/facebook/facebooknotificationinterface.h \
    $$PWD/../social/src/facebook/facebooknotificationinterface_p.h \
    $$PWD/../social/src/facebook/facebookpageinterface.h \
    $$PWD/../social/src/facebook/facebookpageinterface_p.h \
    $$PWD/../social/src/facebook/facebookphotointerface.h \
    $$PWD/../social/src/facebook/facebookphotointerface_p.h \
    $$PWD/../social/src/facebook/facebookpostinterface.h \
    $$PWD/../social/src/facebook/facebookpostinterface_p.h \
    $$PWD/../social/src/facebook/facebookthreadinterface.h \
    $$PWD/../social/src/facebook/facebookthreadinterface_p.h \
    $$PWD/../social/src/facebook/facebookuserinterface.h \
    $$PWD/../social/src/facebook/facebookuserinterface_p.h \
    $$PWD/../social/src/facebook/facebookcoverinterface.h \
    $$PWD/../social/src/facebook/facebooklikeinterface.h \
    $$PWD/../social/src/facebook/facebooknametaginterface.h \
    $$PWD/../social/src/facebook/facebookphotoimageinterface.h \
    $$PWD/../social/src/facebook/facebookphototaginterface.h \
    $$PWD/../social/src/facebook/facebookpostactioninterface.h \
    $$PWD/../social/src/facebook/facebookpostpropertyinterface.h \
    $$PWD/../social/src/facebook/facebookusercoverinterface.h \
    $$PWD/../social/src/facebook/facebookuserpictureinterface.h \
    $$PWD/../social/src/facebook/facebookitemfilterinterface.h \
    $$PWD/../social/src/facebook/facebookrelateddatafilterinterface.h

SOURCES += \
    $$PWD/../social/src/facebook/facebookinterface.cpp \
    $$PWD/../social/src/facebook/facebookobjectreferenceinterface.cpp \
    $$PWD/../social/src/facebook/facebookalbuminterface.cpp \
    $$PWD/../social/src/facebook/facebookcommentinterface.cpp \
    $$PWD/../social/src/facebook/facebookeventinterface.cpp \
    $$PWD/../social/src/facebook/facebookgroupinterface.cpp \
    $$PWD/../social/src/facebook/facebooknotificationinterface.cpp \
    $$PWD/../social/src/facebook/facebookpageinterface.cpp \
    $$PWD/../social/src/facebook/facebookphotointerface.cpp \
    $$PWD/../social/src/facebook/facebookpostinterface.cpp \
    $$PWD/../social/src/facebook/facebookthreadinterface.cpp \
    $$PWD/../social/src/facebook/facebookuserinterface.cpp \
    $$PWD/../social/src/facebook/facebookcoverinterface.cpp \
    $$PWD/../social/src/facebook/facebooklikeinterface.cpp \
    $$PWD/../social/src/facebook/facebooknametaginterface.cpp \
    $$PWD/../social/src/facebook/facebookphotoimageinterface.cpp \
    $$PWD/../social/src/facebook/facebookphototaginterface.cpp \
    $$PWD/../social/src/facebook/facebookpostactioninterface.cpp \
    $$PWD/../social/src/facebook/facebookpostpropertyinterface.cpp \
    $$PWD/../social/src/facebook/facebookusercoverinterface.cpp \
    $$PWD/../social/src/facebook/facebookuserpictureinterface.cpp \
    $$PWD/../social/src/facebook/facebookitemfilterinterface.cpp \
    $$PWD/../social/src/facebook/facebookrelateddatafilterinterface.cpp

# Friends
INCLUDEPATH += $$PWD/../friends/src
HEADERS += $$PWD/../friends/src/socialextra/alphabeticalsorterinterface.h \
    $$PWD/../friends/src/socialextra/newsfeedfilterinterface.h \
    $$PWD/../friends/src/socialextra/facebookextrapostinterface.h \
    $$PWD/../friends/src/socialextra/typesolverinterface.h \
    $$PWD/../friends/src/socialextra/filterablefacebookrelateddatafilterinterface.h \
    $$PWD/../friends/src/socialextra/eventfilterinterface.h \
    $$PWD/../friends/src/socialextra/facebookextraeventinterface.h \
    $$PWD/../friends/src/socialextra/facebookextrainterface.h \
    $$PWD/../friends/src/socialextra/commentfilterinterface.h

SOURCES += $$PWD/../friends/src/socialextra/alphabeticalsorterinterface.cpp \
    $$PWD/../friends/src/socialextra/newsfeedfilterinterface.cpp \
    $$PWD/../friends/src/socialextra/facebookextrapostinterface.cpp \
    $$PWD/../friends/src/socialextra/typesolverinterface.cpp \
    $$PWD/../friends/src/socialextra/filterablefacebookrelateddatafilterinterface.cpp \
    $$PWD/../friends/src/socialextra/eventfilterinterface.cpp \
    $$PWD/../friends/src/socialextra/facebookextraeventinterface.cpp \
    $$PWD/../friends/src/socialextra/facebookextrainterface.cpp \
    $$PWD/../friends/src/socialextra/commentfilterinterface.cpp


HEADERS += $$PWD/../friends/src/cachehelper_p.h \
    $$PWD/../friends/src/clientidplugininterface.h \
    $$PWD/../friends/src/tokenmanager.h \
    $$PWD/../friends/src/posthelper.h \
    $$PWD/../friends/src/footerhelper.h \
    $$PWD/../friends/src/notificationshelper.h \
    $$PWD/../friends/src/imagehelper.h \
    $$PWD/../friends/src/imagemanager.h \
    $$PWD/../friends/src/objecthelper_p.h \
    $$PWD/../friends/src/abstractdisplayhelper.h \
    $$PWD/../friends/src/userinfohelper.h \
    $$PWD/../friends/src/datehelper.h \
    $$PWD/../friends/src/defines_p.h
SOURCES += $$PWD/../friends/src/tokenmanager.cpp \
    $$PWD/../friends/src/posthelper.cpp \
    $$PWD/../friends/src/footerhelper.cpp \
    $$PWD/../friends/src/notificationshelper.cpp \
    $$PWD/../friends/src/imagehelper.cpp \
    $$PWD/../friends/src/imagemanager.cpp \
    $$PWD/../friends/src/abstractdisplayhelper.cpp \
    $$PWD/../friends/src/userinfohelper.cpp \
    $$PWD/../friends/src/datehelper.cpp
