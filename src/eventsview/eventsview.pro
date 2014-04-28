TARGET = friends-integration-eventsview
PLUGIN_IMPORT_PATH = org/SfietKonstantin/friends/integration/eventsview

TEMPLATE = lib
CONFIG += qt plugin hide_symbols

target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH
INSTALLS += target

QT += sql qml quick
CONFIG += link_pkgconfig
PKGCONFIG += buteosyncfw5
INCLUDEPATH += ../daemon/

HEADERS += $$PWD/../daemon/friendsjsonpostdatabase.h \
    $$PWD/../daemon/friendspostinterface.h \
    $$PWD/friendspostmodel.h \
    $$PWD/synchelper.h

SOURCES += plugin.cpp \
    $$PWD/../daemon/friendsjsonpostdatabase.cpp \
    $$PWD/../daemon/friendspostinterface.cpp \
    $$PWD/friendspostmodel.cpp \
    $$PWD/synchelper.cpp

qml.files = qmldir
qml.path = $$target.path

eventsview.files = friends-delegate.qml \
    friends-feed.qml \
    friends-update.qml
eventsview.path = /usr/share/lipstick/eventfeed/

OTHER_FILES += $${qml} \
    $${eventsview}

INSTALLS += qml eventsview

include($$PWD/../socialcache.pri)
include($$PWD/../socialcacheqml.pri)


