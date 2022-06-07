#-------------------------------------------------
#
# Project created by QtCreator 2018-07-13T09:30:13
#
#-------------------------------------------------

QT       += core gui network sql printsupport multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = cannachat
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += IRC_STATIC

DEFINES += IRRLICHT
#DEFINES += QT_NO_SSL

CONFIG += C++11
CONFIG += console

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += .
INCLUDEPATH += ./IrcCore
INCLUDEPATH += ./IrcModel
INCLUDEPATH += ./IrcUtil
INCLUDEPATH += ./include
INCLUDEPATH += ./i18n
INCLUDEPATH += ./uchardet
INCLUDEPATH += ./debug
INCLUDEPATH += ./release


unix:MOC_DIR = ./mocs
win32:MOC_DIR = ./mocs

#ifdef IRRLICHT
win32:INCLUDEPATH+=C:\Users\TEST\Downloads\irrlicht-1.8.4\irrlicht-1.8.4\include ../LunaLibs/irrlicht-1.8.4/
else:unix:INCLUDEPATH+= ./ /home/netpipe/gamedev/irrlicht-1.8.4/include ../LunaLibs/irrlicht-1.8.4/include

win32:LIBS += -LC:\Users\TEST\Downloads\irrlicht-1.8.4\irrlicht-1.8.4\lib\Win32-visualstudio -L../LunaLibs/irrlicht-1.8.4/lib/win32-gcc
else:unix:LIBS += -L/home/netpipe/gamedev/irrlicht-1.8.4/lib/Linux -L../LunaLibs/irrlicht-1.8.4/lib/Linux -lQt5Widgets -lQt5Gui -lQt5Core -lXxf86vm -lIrrlicht -lGL -lX11

linux{
LIBS+= -lIrrlicht -lGL -lX11  -lXxf86vm
}


linux2 {
DEFINES += SOUND MEDIAPLAYER
CONFIG +=
QT += multimedia svg dbus network
}


linux {
DEFINES += zlib QUAZIP DOWNLOAD SMTP SOUND DBUS STORAGE FTP MEDIAPLAYER PYTHON FIGLET
QT += multimedia svg dbus network opengl
LIBS += -lpython2.7
CONFIG += barcodescan
#unix:!macx:CONFIG += barcodescan
barcodescan{
LIBS+= -lpng -ljpeg
DEFINES += BARCODE
}
//INCLUDEPATH

}

win32 {
DEFINES += SOUND MEDIAPLAYER
CONFIG +=
QT += multimedia svg dbus network
}


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    src/figlet/crc.cpp \
    IrcCore/irc.cpp \
    IrcCore/irccommand.cpp \
    IrcCore/ircconnection.cpp \
    IrcCore/irccore.cpp \
    IrcCore/ircfilter.cpp \
    IrcCore/ircmessage.cpp \
    IrcCore/ircmessage_p.cpp \
    IrcCore/ircmessagecomposer.cpp \
    IrcCore/ircmessagedecoder.cpp \
    IrcCore/ircnetwork.cpp \
    IrcCore/ircprotocol.cpp \
    IrcModel/ircbuffer.cpp \
    IrcModel/ircbuffermodel.cpp \
    IrcModel/ircchannel.cpp \
    IrcModel/ircmodel.cpp \
    IrcModel/ircuser.cpp \
    IrcModel/ircusermodel.cpp \
    IrcUtil/irccommandparser.cpp \
    IrcUtil/irccommandqueue.cpp \
    IrcUtil/irccompleter.cpp \
    IrcUtil/irclagtimer.cpp \
    IrcUtil/ircpalette.cpp \
    IrcUtil/irctextformat.cpp \
    IrcUtil/irctoken.cpp \
    IrcUtil/ircutil.cpp \
    ircclient.cpp \
    ircmessageformatter.cpp \
    irrlichtwidget.cpp \
    irrutil.cpp \
    servers.cpp \
    settings.cpp \
    qstylesheetmanager.cpp\
    src/encryption/rsa/Rsa.cpp \
    src/encryption/Qt-AES/qaesencryption.cpp \
    src/encryption/simpleCrypt/simplecrypt.cpp \
    src/mpqt/playlistManager.cpp \
    src/smtp/smtpclient.cpp \
    src/smtp/quotedprintable.cpp \
    src/smtp/mimetext.cpp \
    src/smtp/mimepart.cpp \
    src/smtp/mimemultipart.cpp \
    src/smtp/mimemessage.cpp \
    src/smtp/mimeinlinefile.cpp \
    src/smtp/mimehtml.cpp \
    src/smtp/mimefile.cpp \
    src/smtp/mimecontentformatter.cpp \
    src/smtp/mimeattachment.cpp \
    src/smtp/emailaddress.cpp \
    src/QRCode/QrCode.cpp \
    src/Barcode/functii.cpp \
    src/Barcode/quirc/lib/decode.c \
    src/Barcode/quirc/lib/identify.c \
    src/Barcode/quirc/lib/quirc.c \
    src/Barcode/quirc/lib/version_db.c \
    src/ftp-server/dataconnection.cpp \
    src/ftp-server/debuglogdialog.cpp \
    src/ftp-server/ftpcommand.cpp \
    src/ftp-server/ftpcontrolconnection.cpp \
    src/ftp-server/ftplistcommand.cpp \
    src/ftp-server/ftpretrcommand.cpp \
    src/ftp-server/ftpserver.cpp \
    src/ftp-server/ftpstorcommand.cpp \
    src/ftp-server/sslserver.cpp \
    src/ftp-server/ftpgui.cpp \
    src/quazip/JlCompress.cpp \
    src/quazip/qioapi.cpp \
    src/quazip/quaadler32.cpp \
    src/quazip/quacrc32.cpp \
    src/quazip/quagzipfile.cpp \
    src/quazip/quaziodevice.cpp \
    src/quazip/quazip.cpp \
    src/quazip/quazipdir.cpp \
    src/quazip/quazipfile.cpp \
    src/quazip/quazipfileinfo.cpp \
    src/quazip/quazipnewinfo.cpp \
    src/quazip/unzip.c \
    src/quazip/zip.c \
    src/zlib/adler32.c \
    src/zlib/compress.c \
    src/zlib/crc32.c \
    src/zlib/deflate.c \
    src/zlib/gzclose.c \
    src/zlib/gzlib.c \
    src/zlib/gzread.c \
    src/zlib/gzwrite.c \
    src/zlib/infback.c \
    src/zlib/inffast.c \
    src/zlib/inflate.c \
    src/zlib/inftrees.c \
    src/zlib/trees.c \
    src/zlib/uncompr.c \
    src/zlib/zutil.c \
    src/oglwidget.cpp \
    src/downloadmanager.cpp \
    src/mpqt/track.cpp \
    src/mpqt/trackdelegate.cpp \
    src/mpqt/trackmodel.cpp \
    src/mpqt/trackwidget.cpp \
    src/mpqt/audiothread.cpp \
    src/figlet/zipio.c \
    src/figlet/inflate2.cpp \
    voiceChat/voice-over-lan/buffer.cpp \
    voiceChat/voice-over-lan/messenger.cpp \
    voiceChat/voice-over-lan/voiceio.cpp \
    voiceChat/voice-over-lan/voicesocket.cpp \
    voiceChat/voice-over-lan/xmain3.cpp


HEADERS += \
    IrcCore/Irc \
    IrcCore/IrcCommandFilter \
    IrcCore/IrcCore \
    IrcCore/irc.h \
    IrcCore/irccommand.h \
    IrcCore/irccommand_p.h \
    IrcCore/irccommandfilter.h \
    IrcCore/ircconnection.h \
    IrcCore/ircconnection_p.h \
    IrcCore/irccore.h \
    IrcCore/ircdebug_p.h \
    IrcCore/ircfilter.h \
    IrcCore/ircglobal.h \
    IrcCore/ircmessage.h \
    IrcCore/ircmessage_p.h \
    IrcCore/ircmessagecomposer_p.h \
    IrcCore/ircmessagedecoder_p.h \
    IrcCore/ircmessagefilter.h \
    IrcCore/ircnetwork.h \
    IrcCore/ircnetwork_p.h \
    IrcCore/ircprotocol.h \
        mainwindow.h \
    IrcCore/irc.h \
    IrcCore/IrcCommand \
    IrcCore/irccommand.h \
    IrcCore/irccommand_p.h \
    IrcCore/irccommandfilter.h \
    IrcCore/IrcConnection \
    IrcCore/ircconnection.h \
    IrcCore/ircconnection_p.h \
    IrcCore/irccore.h \
    IrcCore/ircdebug_p.h \
    IrcCore/IrcFilter \
    IrcCore/ircfilter.h \
    IrcCore/IrcGlobal \
    IrcCore/ircglobal.h \
    IrcCore/IrcMessage \
    IrcCore/ircmessage.h \
    IrcCore/ircmessage_p.h \
    IrcCore/ircmessagecomposer_p.h \
    IrcCore/ircmessagedecoder_p.h \
    IrcCore/IrcMessageFilter \
    IrcCore/ircmessagefilter.h \
    IrcCore/IrcNetwork \
    IrcCore/ircnetwork.h \
    IrcCore/ircnetwork_p.h \
    IrcCore/IrcProtocol \
    IrcCore/ircprotocol.h \
    IrcModel/ircbuffer.h \
    IrcModel/ircbuffer_p.h \
    IrcModel/ircbuffermodel.h \
    IrcModel/ircbuffermodel_p.h \
    IrcModel/ircchannel.h \
    IrcModel/ircchannel_p.h \
    IrcModel/ircmodel.h \
    IrcModel/ircuser.h \
    IrcModel/ircuser_p.h \
    IrcModel/ircusermodel.h \
    IrcModel/ircusermodel_p.h \
    IrcModel/IrcBuffer \
    IrcModel/IrcBufferModel \
    IrcModel/IrcChannel \
    IrcModel/IrcModel \
    IrcModel/IrcUser \
    IrcModel/IrcUserModel \
    IrcUtil/IrcCommandParser \
    IrcUtil/irccommandparser.h \
    IrcUtil/irccommandparser_p.h \
    IrcUtil/IrcCommandQueue \
    IrcUtil/irccommandqueue.h \
    IrcUtil/irccommandqueue_p.h \
    IrcUtil/IrcCompleter \
    IrcUtil/irccompleter.h \
    IrcUtil/IrcLagTimer \
    IrcUtil/irclagtimer.h \
    IrcUtil/irclagtimer_p.h \
    IrcUtil/IrcPalette \
    IrcUtil/ircpalette.h \
    IrcUtil/IrcTextFormat \
    IrcUtil/irctextformat.h \
    IrcUtil/irctoken_p.h \
    IrcUtil/IrcUtil \
    IrcUtil/ircutil.h \
    ircclient.h \
    ircmessageformatter.h \
    irrlichtwidget.h \
    irrutil.h \
    servers.h \
    settings.h \
    qstylesheetmanager.h \
    movemodelanimator.h \
    loadtheme.h\
    src/encryption/rsa/Rsa.h \
    src/encryption/Qt-AES/qaesencryption.h \
    src/encryption/encryption.h \
    src/mpqt/playlistManager.h \
    src/smtp/SmtpMime \
    src/smtp/smtpexports.h \
    src/smtp/smtpclient.h \
    src/smtp/quotedprintable.h \
    src/smtp/mimetext.h \
    src/smtp/mimepart.h \
    src/smtp/mimemultipart.h \
    src/smtp/mimemessage.h \
    src/smtp/mimeinlinefile.h \
    src/smtp/mimehtml.h \
    src/smtp/mimefile.h \
    src/smtp/mimecontentformatter.h \
    src/smtp/mimeattachment.h \
    src/smtp/emailaddress.h \
    src/encryption/simpleCrypt/simplecrypt.h \
    src/coingenerator.h \
    src/QRCode/QrCode.hpp \
    src/Barcode/functii.h \
    src/dbus/dbushandler.h \
    src/Barcode/quirc/lib/quirc.h \
    src/Barcode/quirc/lib/quirc_internal.h \
    src/Barcode/quirc/tests/dbgutil.h \
    src/Barcode/quirc/tests/inspect.h \
    src/email.h \
    src/ssl.h \
    src/ftp-server/dataconnection.h \
    src/ftp-server/debuglogdialog.h \
    src/ftp-server/ftpcommand.h \
    src/ftp-server/ftpcontrolconnection.h \
    src/ftp-server/ftplistcommand.h \
    src/ftp-server/ftpretrcommand.h \
    src/ftp-server/ftpserver.h \
    src/ftp-server/ftpstorcommand.h \
    src/ftp-server/sslserver.h \
    src/ftp-server/ftpgui.h \
    src/quazip/ioapi.h \
    src/quazip/JlCompress.h \
    src/quazip/quaadler32.h \
    src/quazip/quachecksum32.h \
    src/quazip/quacrc32.h \
    src/quazip/quagzipfile.h \
    src/quazip/quaziodevice.h \
    src/quazip/quazip.h \
    src/quazip/quazipdir.h \
    src/quazip/quazipfile.h \
    src/quazip/quazipfileinfo.h \
    src/quazip/quazip_global.h \
    src/quazip/quazipnewinfo.h \
    src/quazip/unzip.h \
    src/quazip/zip.h \
    src/zlib/crc32.h \
    src/zlib/deflate.h \
    src/zlib/gzguts.h \
    src/zlib/inffast.h \
    src/zlib/inffixed.h \
    src/zlib/inflate.h \
    src/zlib/inftrees.h \
    src/zlib/trees.h \
    src/zlib/zconf.h \
    src/zlib/zlib.h \
    src/zlib/zutil.h \
    src/oglwidget.h \
    src/downloadmanager.h \
    src/mpqt/scanner.h \
    src/mpqt/track.h \
    src/mpqt/trackdelegate.h \
    src/mpqt/trackmodel.h \
    src/mpqt/trackwidget.h \
    src/player.h \
    src/mpqt/audiothread.h \
    src/Barcode/scanner.h \
    src/figlet/figlet.h \
    src/python/python.h \
    voiceChat/voice-over-lan/buffer.h \
    voiceChat/voice-over-lan/messenger.h \
    voiceChat/voice-over-lan/voiceio.h \
    voiceChat/voice-over-lan/voicesocket.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    mainwindow.ui \
    servers.ui \
    settings.ui \
    servers.ui \
    settings.ui\
    src/ftp-server/debuglogdialog.ui \
    src/ftp-server/ftpgui.ui

SUBDIRS += \
    voiceChat/voice-over-lan/QtIntercom.pro

