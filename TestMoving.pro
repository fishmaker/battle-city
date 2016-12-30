#-------------------------------------------------
#
# Project created by QtCreator 2016-12-20T14:57:56
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestMoving
TEMPLATE = app

INCLUDEPATH += include


SOURCES += \
    main.cpp\
    src/gameengine.cpp \
    src/tank.cpp \
    src/brick.cpp \
    src/bullet.cpp \
    src/armor.cpp \
    src/gameobject.cpp \
    src/bot.cpp

HEADERS  += \
    include/game/gameengine.h \
    include/game/tank.h \
    include/game/brick.h \
    include/game/bullet.h \
    include/game/common.h \
    include/game/armor.h \
    include/game/gameobject.h \
    include/game/bot.h

RESOURCES += \
    res.qrc
