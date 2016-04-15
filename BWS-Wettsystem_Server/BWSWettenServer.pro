#-------------------------------------------------
#
# Project created by QtCreator 2016-03-07T09:23:00
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BWSWettenServer
TEMPLATE = app


SOURCES += main.cpp\
        frmmain.cpp \
    login.cpp \
    dbhandler.cpp \
    accountliste.cpp \
    account.cpp \
    event.cpp \
    eventliste.cpp \
    eventspieler.cpp \
    eventspielerliste.cpp \
    tipp.cpp \
    tippliste.cpp \
    wette.cpp \
    wettenliste.cpp \
    bwsprotokoll.cpp \
    spielerinfo.cpp \
    spielinfo.cpp \
    tipinfo.cpp \
    verbindung.cpp \
    serverhandler.cpp \
    eventinfo.cpp \
    info.cpp

HEADERS  += frmmain.h \
    login.h \
    dbhandler.h \
    accountliste.h \
    account.h \
    event.h \
    eventliste.h \
    eventspieler.h \
    eventspielerliste.h \
    tipp.h \
    tippliste.h \
    wette.h \
    wettenliste.h \
    bwsprotokoll.h \
    spielerinfo.h \
    spielinfo.h \
    tipinfo.h \
    verbindung.h \
    serverhandler.h \
    eventinfo.h \
    info.h

FORMS    += frmmain.ui \
    login.ui \
    info.ui

RC_ICONS = 0001.ico
ICON = 0001.icns
