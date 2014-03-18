TEMPLATE = subdirs
OTHER_FILES = post.sh \
    preun.sh \
    addsocial.patch \
    rmsocial.patch

qmldir.files += $$OTHER_FILES
qmldir.path +=  /opt/friends-integration/
INSTALLS += qmldir
