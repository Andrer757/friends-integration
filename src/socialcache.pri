DEFINES += 'PRIVILEGED_DATA_DIR=\'QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + \"/system/privileged/\"\''

INCLUDEPATH += $$PWD/../libsocialcache/src/lib/

HEADERS += \
    $$PWD/../libsocialcache/src/lib/semaphore_p.h \
    $$PWD/../libsocialcache/src/lib/socialsyncinterface.h \
    $$PWD/../libsocialcache/src/lib/abstractsocialcachedatabase.h \
    $$PWD/../libsocialcache/src/lib/abstractsocialcachedatabase_p.h \
    $$PWD/../libsocialcache/src/lib/abstractsocialpostcachedatabase.h

SOURCES += \
    $$PWD/../libsocialcache/src/lib/semaphore_p.cpp \
    $$PWD/../libsocialcache/src/lib/socialsyncinterface.cpp \
    $$PWD/../libsocialcache/src/lib/abstractsocialcachedatabase.cpp \
    $$PWD/../libsocialcache/src/lib/abstractsocialpostcachedatabase.cpp
