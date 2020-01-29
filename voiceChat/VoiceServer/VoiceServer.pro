#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T16:30:14
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VoiceServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp

HEADERS  += mainwindow.h \
    server.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

