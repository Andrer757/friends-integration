#!/bin/sh
sed -i 's/import harbour.friends 1.0/import org.SfietKonstantin.friends.integration 1.0/g' /usr/lib/qt5/qml/org/SfietKonstantin/friends/integration/*.qml
sed -i 's/import harbour.friends.social 1.0/import org.SfietKonstantin.friends.integration 1.0/g' /usr/lib/qt5/qml/org/SfietKonstantin/friends/integration/*.qml
sed -i 's/import harbour.friends.social.extra 1.0/import org.SfietKonstantin.friends.integration 1.0/g' /usr/lib/qt5/qml/org/SfietKonstantin/friends/integration/*.qml

# Patching system files
cd /usr/lib/qt5/qml/Sailfish/Contacts/
patch -p1 < /opt/friends-integration/addsocial.patch > /dev/null
exit 0
