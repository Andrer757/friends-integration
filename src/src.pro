TARGET = friends-integration
PLUGIN_IMPORT_PATH = org/SfietKonstantin/friends/integration

TEMPLATE = lib
CONFIG += qt plugin hide_symbols #link_pkgconfig
#PKGCONFIG +=  Qt5Contacts

target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH
INSTALLS += target

QT += qml network contacts

HEADERS += \
    $$PWD/nemocontactbridge.h

SOURCES += \
    $$PWD/plugin.cpp \
    $$PWD/nemocontactbridge.cpp

OTHER_FILES += qmldir \
    FriendsUserAttachedPage.qml

qmlFiles.files += $$OTHER_FILES
qmlFiles.path +=  $$target.path
INSTALLS += qmlFiles
