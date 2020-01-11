#include "mainwindow.h"
#include "ircclient.h"
#include "ui_mainwindow.h"

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
