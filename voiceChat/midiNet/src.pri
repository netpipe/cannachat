# qmidinet.pri
#
PREFIX  = /usr/local
BINDIR  = /usr/local/bin
LIBDIR  = /usr/local/lib
DATADIR = /usr/local/share
MANDIR  = /usr/local/share/man

VERSION = 0.5.1
CONFIG += release

INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5 
LIBS += -L/usr/lib/x86_64-linux-gnu  -lasound -ljack

# Extra optimization flags
QMAKE_CXXFLAGS +=  -I/usr/include/alsa 
QMAKE_LFLAGS += 
