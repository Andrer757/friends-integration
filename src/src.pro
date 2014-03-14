TARGET = friends-integration
PLUGIN_IMPORT_PATH = org/SfietKonstantin/friends/integration

TEMPLATE = lib
CONFIG += qt plugin hide_symbols

target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH
INSTALLS += target

qmldir.files += qmldir
qmldir.path +=  $$target.path
INSTALLS += qmldir

QT += qml network

SOURCES += \
    $$PWD/plugin.cpp

OTHER_FILES += qmldir
