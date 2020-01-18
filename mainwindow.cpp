#include "mainwindow.h"
#include "ircclient.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "servers.h"
#include "ui_servers.h"
#include "ui_settings.h"

#include "qmessagebox.h"
#include "qprocess.h"
#include "qstring.h"
#include "qcoreapplication.h"
#include "qobject.h"
#include <QDebug>
#include <QEvent>

ServerDlg *serversw;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
       ui->setupUi(this);
    setCentralWidget(new IrcClient());

//    ui->setupUi(servers);
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_actionExit_triggered()
{
QApplication::quit();
}

void MainWindow::on_actionNew_triggered()
{
    serversw = new ServerDlg();
    serversw->show();
}

void MainWindow::on_actionClose_triggered()
{

}

void MainWindow::on_actionRestore_triggered()
{

}
