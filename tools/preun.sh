#!/bin/sh
cd /usr/lib/qt5/qml/Sailfish/Contacts/
patch -p1 < /opt/friends-integration/rmsocial.patch > /dev/null
exit 0
