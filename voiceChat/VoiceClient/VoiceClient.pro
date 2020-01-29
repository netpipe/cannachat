#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T16:54:14
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VoiceClient
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui

CONFIG += mobility
MOBILITY = 

