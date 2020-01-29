#-------------------------------------------------
#
# Project created by QtCreator 2016-08-26T12:48:42
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VoiceChatClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    audioinput.cpp

HEADERS  += mainwindow.h \
    audioinput.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

