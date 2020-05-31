#include "mainwindow.h"
#include <QApplication>
#include "qstylesheetmanager.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QStyleSheetManager::loadStyleSheet("Resource/themes/qdarkstyle/qdarkstyle.qss");
    return a.exec();
}
