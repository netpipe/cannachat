#include "mainwindow.h"
#include <QApplication>
#include "qstylesheetmanager.h"
#include <QSplashScreen>
#include <QFile>

#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setApplicationName("Cannachat");
    a.setApplicationVersion("0.1");
   // a.setQuitOnLastWindowClosed(false);
    QPixmap m( "./Resource/420-cannabis.jpg");

//    QCommandLineParser parser;
//    parser.setApplicationDescription("Voice over LAN");
//    parser.addHelpOption();
//    parser.addVersionOption();
//    parser.addPositionalArgument("address", QApplication::translate("main", "Address of the counterpart"));

//    parser.process(a);
//    QString address("");
//    if(!parser.positionalArguments().isEmpty())
//    {
//        address = parser.positionalArguments().at(0);
//    }


    MainWindow w;

    QSplashScreen splash(m);
    splash.show();

    splash.finish(&w);

    w.show();

   // QFile file("Resource/themes/qdarkstyle/qdarkstyle.qss");    // Modified by RCU 2016.07.17
   // file.open(QFile::ReadOnly);
   // QString styleSheet = QLatin1String(file.readAll());


    return a.exec();
}
