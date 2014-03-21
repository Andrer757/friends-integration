TARGET = friends-integration
PLUGIN_IMPORT_PATH = org/SfietKonstantin/friends/integration

TEMPLATE = lib
CONFIG += qt plugin hide_symbols

target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH
INSTALLS += target

QT += contacts qml network quick quick-private dbus

HEADERS += \
    $$PWD/nemocontactbridge.h \
    friendsinvoker.h \
    friendsclientidprovider.h

SOURCES += \
    $$PWD/plugin.cpp \
    $$PWD/nemocontactbridge.cpp \
    friendsinvoker.cpp \
    friendsclientidprovider.cpp

OTHER_FILES += qmldir \
    FriendsUserAttachedPage.qml \
    ../friends/src/qml/UiConstants.js \
    ../friends/src/qml/StateIndicator.qml \
    ../friends/src/qml/FacebookPicture.qml \
    ../friends/src/qml/FacebookImage.qml \
    ../friends/src/qml/CoverImage.qml \
    ../friends/src/qml/PostDelegate.qml

qmlFiles.files += $$OTHER_FILES
qmlFiles.path +=  $$target.path
INSTALLS += qmlFiles

include(social.pri)
