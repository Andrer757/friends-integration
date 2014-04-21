TARGET = friends-integration
PLUGIN_IMPORT_PATH = org/SfietKonstantin/friends/integration

TEMPLATE = lib
CONFIG += qt plugin hide_symbols

target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH
INSTALLS += target

QT += qml network quick quick-private dbus

HEADERS += $$PWD/friendsclientidprovider.h

SOURCES += $$PWD/plugin.cpp \
    $$PWD/friendsclientidprovider.cpp

OTHER_FILES += qmldir \
    ../../friends/src/qml/*

qmlFiles.files += $$OTHER_FILES
qmlFiles.path +=  $$target.path
INSTALLS += qmlFiles

include(../social.pri)
