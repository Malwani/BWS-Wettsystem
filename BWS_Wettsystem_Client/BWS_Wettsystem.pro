#-------------------------------------------------
#
# Project created by QtCreator 2016-03-03T11:50:26
#
#-------------------------------------------------

QT       += core gui network sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BWS_Wettsystem
TEMPLATE = app


SOURCES += main.cpp\
        frmmain.cpp \
    login.cpp \
    register.cpp \
    verbindungsaufbau.cpp \
    bwsprotokoll.cpp \
    clienthandler.cpp \
    eventinfo.cpp \
    spielerinfo.cpp \
    tipinfo.cpp \
    spielinfo.cpp \
    activeuser.cpp \
    info.cpp

HEADERS  += frmmain.h \
    login.h \
    register.h \
    verbindungsaufbau.h \
    bwsprotokoll.h \
    clienthandler.h \
    eventinfo.h \
    spielerinfo.h \
    spielinfo.h \
    tipinfo.h \
    activeuser.h \
    info.h

FORMS    += frmmain.ui \
    login.ui \
    register.ui \
    verbindungsaufbau.ui \
    info.ui

RC_ICONS = 0001.ico
ICON = 0001.icns

RESOURCES += \
    bwswettsystemlogo.qrc
