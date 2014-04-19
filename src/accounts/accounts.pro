TEMPLATE = aux
OTHER_FILES += friends.provider \
    friends.qml

provider.files += friends.provider
provider.path +=  /usr/share/accounts/providers/

ui.files += friends.qml
provider.path +=  /usr/share/accounts/ui
INSTALLS += provider
