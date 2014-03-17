TEMPLATE = subdirs
OTHER_FILES = post.sh \
    preun.sh \
    ContactCardPage.qml

qmldir.files += $$OTHER_FILES
qmldir.path +=  /opt/friends-integration/
INSTALLS += qmldir
