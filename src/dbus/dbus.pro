TEMPLATE = aux

# DBus service
dbusService.files = harbour.friends.service
dbusService.path = /usr/share/dbus-1/services/
INSTALLS += dbusService

# DBus interface
dbusInterface.files = ../../friends/src/dbus/harbour.friends.xml
dbusInterface.path = /usr/share/dbus-1/interfaces/
INSTALLS += dbusInterface

