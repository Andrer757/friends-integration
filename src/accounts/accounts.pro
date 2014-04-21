TEMPLATE = aux
OTHER_FILES += friends.provider \
    friends-microblog.service \
    friends.qml

provider.files += friends.provider
provider.path +=  /usr/share/accounts/providers/

services.files += friends-microblog.service
services.path += /usr/share/accounts/services/

ui.files += friends.qml
ui.path +=  /usr/share/accounts/ui
INSTALLS += provider services ui
