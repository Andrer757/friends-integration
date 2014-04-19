equals(QT_MAJOR_VERSION, 5): DEFINES += QT_VERSION_5

system(sed -i \'s/import harbour.friends 1.0/import org.SfietKonstantin.friends.integration 1.0/g\' ../../friends/src/qml/*.qml)
system(sed -i \'s/import harbour.friends.social 1.0/import org.SfietKonstantin.friends.integration 1.0/g\' ../../friends/src/qml/*.qml)
system(sed -i \'s/import harbour.friends.social.extra 1.0/import org.SfietKonstantin.friends.integration 1.0/g\' ../../friends/src/qml/*.qml)

INCLUDEPATH += ../../social/src/

HEADERS += \
    ../../social/src/contentiteminterface.h \
    ../../social/src/contentiteminterface_p.h \
    ../../social/src/filterinterface.h \
    ../../social/src/filterinterface_p.h \
    ../../social/src/sorterinterface.h \
    ../../social/src/identifiablecontentiteminterface.h \
    ../../social/src/identifiablecontentiteminterface_p.h \
    ../../social/src/socialnetworkinterface.h \
    ../../social/src/socialnetworkinterface_p.h \
    ../../social/src/socialnetworkmodelinterface.h \
    ../../social/src/socialnetworkmodelinterface_p.h

SOURCES += \
    ../../social/src/contentiteminterface.cpp \
    ../../social/src/filterinterface.cpp \
    ../../social/src/sorterinterface.cpp \
    ../../social/src/identifiablecontentiteminterface.cpp \
    ../../social/src/socialnetworkinterface.cpp \
    ../../social/src/socialnetworkmodelinterface.cpp

# Facebook
HEADERS += \
    ../../social/src/facebook/facebookontology_p.h \
    ../../social/src/facebook/facebookinterface.h \
    ../../social/src/facebook/facebookinterface_p.h \
    ../../social/src/facebook/facebookobjectreferenceinterface.h \
    ../../social/src/facebook/facebookobjectreferenceinterface_p.h \
    ../../social/src/facebook/facebookalbuminterface.h \
    ../../social/src/facebook/facebookalbuminterface_p.h \
    ../../social/src/facebook/facebookcommentinterface.h \
    ../../social/src/facebook/facebookcommentinterface_p.h \
    ../../social/src/facebook/facebookeventinterface.h \
    ../../social/src/facebook/facebookeventinterface_p.h \
    ../../social/src/facebook/facebookgroupinterface.h \
    ../../social/src/facebook/facebookgroupinterface_p.h \
    ../../social/src/facebook/facebooknotificationinterface.h \
    ../../social/src/facebook/facebooknotificationinterface_p.h \
    ../../social/src/facebook/facebookpageinterface.h \
    ../../social/src/facebook/facebookpageinterface_p.h \
    ../../social/src/facebook/facebookphotointerface.h \
    ../../social/src/facebook/facebookphotointerface_p.h \
    ../../social/src/facebook/facebookpostinterface.h \
    ../../social/src/facebook/facebookpostinterface_p.h \
    ../../social/src/facebook/facebookthreadinterface.h \
    ../../social/src/facebook/facebookthreadinterface_p.h \
    ../../social/src/facebook/facebookuserinterface.h \
    ../../social/src/facebook/facebookuserinterface_p.h \
    ../../social/src/facebook/facebookcoverinterface.h \
    ../../social/src/facebook/facebooklikeinterface.h \
    ../../social/src/facebook/facebooknametaginterface.h \
    ../../social/src/facebook/facebookphotoimageinterface.h \
    ../../social/src/facebook/facebookphototaginterface.h \
    ../../social/src/facebook/facebookpostactioninterface.h \
    ../../social/src/facebook/facebookpostpropertyinterface.h \
    ../../social/src/facebook/facebookusercoverinterface.h \
    ../../social/src/facebook/facebookuserpictureinterface.h \
    ../../social/src/facebook/facebookitemfilterinterface.h \
    ../../social/src/facebook/facebookrelateddatafilterinterface.h

SOURCES += \
    ../../social/src/facebook/facebookinterface.cpp \
    ../../social/src/facebook/facebookobjectreferenceinterface.cpp \
    ../../social/src/facebook/facebookalbuminterface.cpp \
    ../../social/src/facebook/facebookcommentinterface.cpp \
    ../../social/src/facebook/facebookeventinterface.cpp \
    ../../social/src/facebook/facebookgroupinterface.cpp \
    ../../social/src/facebook/facebooknotificationinterface.cpp \
    ../../social/src/facebook/facebookpageinterface.cpp \
    ../../social/src/facebook/facebookphotointerface.cpp \
    ../../social/src/facebook/facebookpostinterface.cpp \
    ../../social/src/facebook/facebookthreadinterface.cpp \
    ../../social/src/facebook/facebookuserinterface.cpp \
    ../../social/src/facebook/facebookcoverinterface.cpp \
    ../../social/src/facebook/facebooklikeinterface.cpp \
    ../../social/src/facebook/facebooknametaginterface.cpp \
    ../../social/src/facebook/facebookphotoimageinterface.cpp \
    ../../social/src/facebook/facebookphototaginterface.cpp \
    ../../social/src/facebook/facebookpostactioninterface.cpp \
    ../../social/src/facebook/facebookpostpropertyinterface.cpp \
    ../../social/src/facebook/facebookusercoverinterface.cpp \
    ../../social/src/facebook/facebookuserpictureinterface.cpp \
    ../../social/src/facebook/facebookitemfilterinterface.cpp \
    ../../social/src/facebook/facebookrelateddatafilterinterface.cpp

# Friends
INCLUDEPATH += ../../friends/src
HEADERS += ../../friends/src/socialextra/alphabeticalsorterinterface.h \
    ../../friends/src/socialextra/newsfeedfilterinterface.h \
    ../../friends/src/socialextra/facebookextrapostinterface.h \
    ../../friends/src/socialextra/typesolverinterface.h \
    ../../friends/src/socialextra/filterablefacebookrelateddatafilterinterface.h \
    ../../friends/src/socialextra/eventfilterinterface.h \
    ../../friends/src/socialextra/facebookextraeventinterface.h \
    ../../friends/src/socialextra/facebookextrainterface.h \
    ../../friends/src/socialextra/commentfilterinterface.h

SOURCES += ../../friends/src/socialextra/alphabeticalsorterinterface.cpp \
    ../../friends/src/socialextra/newsfeedfilterinterface.cpp \
    ../../friends/src/socialextra/facebookextrapostinterface.cpp \
    ../../friends/src/socialextra/typesolverinterface.cpp \
    ../../friends/src/socialextra/filterablefacebookrelateddatafilterinterface.cpp \
    ../../friends/src/socialextra/eventfilterinterface.cpp \
    ../../friends/src/socialextra/facebookextraeventinterface.cpp \
    ../../friends/src/socialextra/facebookextrainterface.cpp \
    ../../friends/src/socialextra/commentfilterinterface.cpp


HEADERS += ../../friends/src/cachehelper_p.h \
    ../../friends/src/clientidplugininterface.h \
    ../../friends/src/tokenmanager.h \
    ../../friends/src/posthelper.h \
    ../../friends/src/footerhelper.h \
    ../../friends/src/notificationshelper.h \
    ../../friends/src/imagehelper.h \
    ../../friends/src/imagemanager.h \
    ../../friends/src/objecthelper_p.h \
    ../../friends/src/abstractdisplayhelper.h \
    ../../friends/src/userinfohelper.h \
    ../../friends/src/datehelper.h \
    ../../friends/src/defines_p.h
SOURCES += ../../friends/src/tokenmanager.cpp \
    ../../friends/src/posthelper.cpp \
    ../../friends/src/footerhelper.cpp \
    ../../friends/src/notificationshelper.cpp \
    ../../friends/src/imagehelper.cpp \
    ../../friends/src/imagemanager.cpp \
    ../../friends/src/abstractdisplayhelper.cpp \
    ../../friends/src/userinfohelper.cpp \
    ../../friends/src/datehelper.cpp
