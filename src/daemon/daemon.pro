TEMPLATE = lib
TARGET = friendsintegration-client

CONFIG += link_pkgconfig plugin
PKGCONFIG += buteosyncfw5 nemonotifications-qt5

INCLUDEPATH += /home/sk/Code/SDK/SailfishOS/mersdk/targets/SailfishOS-armv7hl/usr/include/buteosyncfw5/

QT = core dbus network

HEADERS += friendsintegrationclient.h

SOURCES += friendsintegrationclient.cpp

OTHER_FILES += xmls/sync/friendsintegration.xml \
    xmls/client/friendsintegration.xml

target.path = /usr/lib/buteo-plugins-qt5

sync.path = /etc/buteo/profiles/sync
sync.files = xmls/sync/friendsintegration.xml

client.path = /etc/buteo/profiles/client
client.files = xmls/client/friendsintegration.xml

INSTALLS += target sync client

