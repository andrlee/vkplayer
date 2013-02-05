# Auto-generated by IDE. All changes by user will be lost!
# Created at 2/4/13 7:21 PM

BASEDIR = $$_PRO_FILE_PWD_

INCLUDEPATH +=  \
    $$BASEDIR/src/plugin \
    $$BASEDIR/src/plugin/vkontakte \
    $$BASEDIR/src/media \
    $$BASEDIR/src/model \
    $$BASEDIR/src

SOURCES +=  \
    $$BASEDIR/src/Vkplayer.cpp \
    $$BASEDIR/src/main.cpp \
    $$BASEDIR/src/media/invokemanager.cpp \
    $$BASEDIR/src/media/netimagemanager.cpp \
    $$BASEDIR/src/media/netimagetracker.cpp \
    $$BASEDIR/src/media/volumeslider.cpp \
    $$BASEDIR/src/model/vkontakteplaylist.cpp \
    $$BASEDIR/src/plugin/socialconnection.cpp \
    $$BASEDIR/src/plugin/socialconnectplugin.cpp \
    $$BASEDIR/src/plugin/vkontakte/vkontakteconnection.cpp \
    $$BASEDIR/src/plugin/vkontakte/vkontaktemessages.cpp \
    $$BASEDIR/src/plugin/webinterface.cpp

HEADERS +=  \
    $$BASEDIR/src/Vkplayer.hpp \
    $$BASEDIR/src/media/invokemanager.h \
    $$BASEDIR/src/media/netimagemanager.h \
    $$BASEDIR/src/media/netimagetracker.h \
    $$BASEDIR/src/media/volumeslider.h \
    $$BASEDIR/src/model/vkontakteplaylist.h \
    $$BASEDIR/src/plugin/socialconnection.h \
    $$BASEDIR/src/plugin/socialconnectplugin.h \
    $$BASEDIR/src/plugin/vkontakte/vkontakteconnection.h \
    $$BASEDIR/src/plugin/vkontakte/vkontaktemessages.h \
    $$BASEDIR/src/plugin/webinterface.h

CONFIG += precompile_header
PRECOMPILED_HEADER = $$BASEDIR/precompiled.h

lupdate_inclusion {
    SOURCES += \
        $$BASEDIR/../assets/*.qml
}

TRANSLATIONS = \
    $${TARGET}.ts

