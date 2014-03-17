#!/bin/sh
mv /usr/lib/qt5/qml/Sailfish/Contacts/ContactCardPage.qml /opt/friends-integration/ContactCardPage.qml.old
mv /opt/friends-integration/ContactCardPage.qml /usr/lib/qt5/qml/Sailfish/Contacts/ContactCardPage.qml
sed -i 's/import harbour.friends 1.0/import org.SfietKonstantin.friends.integration 1.0/g' /usr/lib/qt5/qml/org/SfietKonstantin/friends/integration/*.qml
sed -i 's/import harbour.friends.social 1.0/import org.SfietKonstantin.friends.integration 1.0/g' /usr/lib/qt5/qml/org/SfietKonstantin/friends/integration/*.qml
sed -i 's/import harbour.friends.social.extra 1.0/import org.SfietKonstantin.friends.integration 1.0/g' /usr/lib/qt5/qml/org/SfietKonstantin/friends/integration/*.qml
