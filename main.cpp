#include "mainwindow.h"
#include <QApplication>
#include "qstylesheetmanager.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

   // QFile file("Resource/themes/qdarkstyle/qdarkstyle.qss");    // Modified by RCU 2016.07.17
   // file.open(QFile::ReadOnly);
   // QString styleSheet = QLatin1String(file.readAll());


    return a.exec();
}
