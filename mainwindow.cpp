#include "mainwindow.h"
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



#include "qstylesheetmanager.h"
#include "loadtheme.h"
//#define IRRLICHT
#ifdef IRRLICHT
#include <QGLWidget>
#include "irrutil.h"
#include "irrlichtwidget.h"
IrrlichtWidget* widget ;
//IrrlichtWidget* widget2 ;
#endif

ServerDlg *serversw;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
       ui->setupUi(this);

//    ui->setupUi(servers);

    //  MainWindow w;
    //  this->setWindowTitle("Main Window");

  //    Dialog1 *newDlg = new Dialog1();
  //    this->hide();
  //    int result = newDlg->exec();
  //    this->show();
  //    delete newDlg;

       ServerDlg *settings = new ServerDlg();
      // settings->show();

       connect(settings, SIGNAL(findtext()), this , SLOT(on_search_clicked()));



#ifdef IRRLICHT
     // widget = new IrrlichtWidget( ui->tabWidget->findChild<QWidget *>("openGLWidget") );
            widget = new IrrlichtWidget( ui->tabWidget->findChild<QWidget *>("irrRenderWidget0") );
     widget->init();
#endif


      //IrcClient *test
              ircwidget = new IrcClient( ui->tabWidget->findChild<QWidget *>("chatwidget"));

//ircwidget->Connections();

//ircwidget[0] = test;
//    ircwidget2 = new IrcClient( ui->tabWidget->findChild<QWidget *>("chatwidget_2"));


    this->resize(1000,700);
    ui->chatwidget->resize(1000,700);

//widget->resizeGL(400,400);
 //     setCentralWidget(widget); //widget

    //  ircwidget->showMaximized();
   //   ircwidget->resize(ui->chatwidget->width(),ui->chatwidget->height());
      //ircwidget->autoRepaint();
//ircwidget->resize();


     // m_button = new QPushButton("My Button", this);
      // set size and location of the button
     // m_button->setGeometry(QRect(QPoint(100, 100),
     // QSize(200, 50)));
     // m_button->setAttribute(Qt::WA_TranslucentBackground);

      // Connect button signal to appropriate slot
    //  connect(m_button, SIGNAL (released()), this, SLOT (handleButton()));


     // QWindow* dlg = new QWindow();
     // dlg->setModal(false);
     // dlg->show();

themeInit();
readsettings();
}

MainWindow::~MainWindow()
{
//   ui->centralWidget->findChild<QWidget *>("irrRenderWidget0"), false);

//    this->irr0->init();

//    this->irr1 = new IrrCoreRenderer(ui->centralWidget->findChild<QWidget *>("irrRenderWidget1"), 1);

//    this->irr1->init();



}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
 //  ircwidget[0].resize(ui->chatwidget->width(),ui->chatwidget->height());
#ifdef IRRLICHT
  // widget->resizeIrrWidget(0, 0, this->size().width(), this->size().height()/2);
  // this->irr1->resizeIrrWidget(this->size().width()/2, 0, this->size().width()/2, this->size().height());
#endif
   // Your code here.
}


void MainWindow::readsettings(){

    QString searchString(":");
    QFile MyFile("settings.txt");
    MyFile.open(QIODevice::ReadWrite);
    QTextStream in (&MyFile);
    QString line;
  //  int ii=0;
    QStringList list;
     //   QList<QString> nums;
    QStringList nums;


    do {
        line = in.readLine();
        searchString=":";
        if (line.contains(searchString)) { //, Qt::CaseSensitive
            // do something
            QRegExp rx("[:]");// match a comma or a space
            list = line.split(rx);
            nums.append(list.at(1).toLatin1());
        }
    } while (!line.isNull());

    //ircwidget->resize(ui->chatwidget->width(),ui->chatwidget->height());

}

void MainWindow::writesettings(){

    QFile file("settings.txt");
      //    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
          if(file.open(QIODevice::ReadWrite | QIODevice::Text))
          {
              QTextStream stream(&file);
              //stream << "CoinName:" << ui->coinname->text() <<'\n';

              file.close();
          }

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

void MainWindow::loadThemeFile(QString path, QString name)
{
//    ui->cmbTheme->addItem(name);
//    m_ThemePathList.append(path);
//    m_ThemeNameList.append(name);

//    QFile themeFile("theme.info");
//    if(themeFile.open(QIODevice::WriteOnly)) {
//        for(int i = 0 ; i < m_ThemePathList.size() ; i++) {
//            themeFile.write(m_ThemePathList.at(i).toUtf8());
//            themeFile.write("\n");
//            themeFile.write(m_ThemeNameList.at(i).toUtf8());
//            themeFile.write("\n");
//        }
//    }
//    QMessageBox::information(NULL, "Notice", "New theme has been successfuly added.");
//    themeLoadDlg->hide();
//    delete themeLoadDlg;
}

void MainWindow::importThemeInfoFromFile()
{
//    QFile themeFile("theme.info");
//    if(themeFile.open(QIODevice::ReadOnly)) {
//        qDebug()<<"Read Theme info success.";
//        while(!themeFile.atEnd()) {
//            QByteArray path = themeFile.readLine();
//            QString strPath = QString::fromUtf8(path).replace("\n", "");
//            m_ThemePathList.append(strPath);

//            QByteArray name = themeFile.readLine();
//            QString strName = QString::fromUtf8(name).replace("\n", "");
//            m_ThemeNameList.append(strName);
//            ui->cmbTheme->addItem(strName);
//        }
//    }
}


void MainWindow::on_pushButton_clicked()
{
ircwidget->Connections();
}
