#-------------------------------------------------
#
# Project created by QtCreator 2016-08-26T12:49:21
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VoiceChatServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp

HEADERS  += mainwindow.h \
    server.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

