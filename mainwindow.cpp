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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
       ui->setupUi(this);
    setCentralWidget(new IrcClient());
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_actionExit_triggered()
{

}

void MainWindow::on_actionNew_triggered()
{

}

void MainWindow::on_actionClose_triggered()
{

}

void MainWindow::on_actionRestore_triggered()
{

}
