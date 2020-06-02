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

      this->setWindowTitle("Cannachat");

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
         //     ircwidget = new IrcClient( ui->tabWidget->findChild<QWidget *>("chatwidget"));

     this->resize(1000,500);

            //  vector<IrcClient*> test2;
     //read settings find autojoin servers
     QString servername = "irc.choopa.net";
     QString channel =  "#cannachat";
     bool ssl = 1;
     int port = 9999;

        //      serverarray.push_back(new IrcClient( ui->tabWidget->findChild<QWidget *>("chatwidget"), ui->nickname->text().toLatin1(), channel.toLatin1(), servername.toLatin1(),port,ssl));
int autostart=0;
     if(autostart){
            for (int i=0; i < serverarray.size() ; i++){
             ui->tabWidget->addTab(serverarray[i], servername.toLatin1());
             //     ui->chatwidget->resize(1000,700);
            }
     }
// read settings find autojoin channels for servers

//ircwidget[0] = test;
//    ircwidget2 = new IrcClient( ui->tabWidget->findChild<QWidget *>("chatwidget_2"));

     if (ui->nickname->text().toLatin1() == "") {
         qDebug() << "setnickname";
         ui->nickname->setText("guest" + QString::number( qrand() % 9999) );
        }

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
    bool settingsexists=false;
//QFile Fout("settings.txt");    if(Fout.exists())    {       settingsexists=true;    }    Fout.close();
if (settingsexists){
    QString searchString(":");
    QFile MyFile("settings.txt");
    MyFile.open(QIODevice::ReadWrite);
    QTextStream in (&MyFile);
    QString line;
  //  int ii=0;
    QStringList list;
     //   QList<QString> nums;
    QStringList nums;
    QString Nick;


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

    QFile MyFile2("servers.txt");
    MyFile2.open(QIODevice::ReadWrite);
    QTextStream in2 (&MyFile2);
list.clear();


    do {
        line = in2.readLine();
        searchString=":";
        if (line.contains(searchString)) { //, Qt::CaseSensitive
            // do something
            QRegExp rx("[:]");// match a comma or a space
            list = line.split(rx);
          //  nums.append(list.at(1).toLatin1());
        }
    } while (!line.isNull());



 ui->nickname->setText(Nick.toLatin1());
}
    //ircwidget->resize(ui->chatwidget->width(),ui->chatwidget->height());

}

void MainWindow::writesettings(){

    QFile file("settings.txt");
          if(file.open(QIODevice::ReadWrite | QIODevice::Text))
          {
              QTextStream stream(&file);

              file.close();
          }

          QFile file2("channels.txt");
            //    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
                if(file2.open(QIODevice::ReadWrite | QIODevice::Text))
                {
                    QTextStream stream(&file);

                    file2.close();
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
//ircwidget->Connections();
}

void MainWindow::on_connect_clicked()
{

QStringList splitlist = ui->serverlist->currentItem()->text().split(":");
    QString servername = splitlist[0].toUtf8();
    qDebug() << splitlist[0].toUtf8();
    QString channel =  "#cannachat";
    int port = splitlist[1].toInt();
        bool ssl = splitlist[2].toInt();

//        QString servername = "irc.choopa.net";
//        QString channel =  "#cannachat";
//        bool ssl = 1;
//        int port = 9999;

                 serverarray.push_back(new IrcClient( ui->tabWidget->findChild<QWidget *>("chatwidget"), ui->nickname->text().toUtf8(), channel.toUtf8(), servername.toUtf8(),port,ssl));
qDebug() << serverarray.size();
                ui->tabWidget->addTab(serverarray[serverarray.size()-1], servername.toLatin1());
//serverarray[serverarray.size()-1]->registerSelf(&serverarray[serverarray.size()-1]);
//serverarray[serverarray.size()-1]->appendText("testing 12345");
}

void MainWindow::on_serverlist_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    //load channels for server maybe use sqlite db
    qDebug() << "channels";
//    QString searchString(":");
//    QFile MyFile("channels.txt");
//    MyFile.open(QIODevice::ReadWrite);
//    QTextStream in (&MyFile);
//    QString line;
//  //  int ii=0;
//    QStringList list;
//     //   QList<QString> nums;
//    QStringList nums;
//    QString Nick;


//    do {
//        line = in.readLine();
//        searchString=":";
//        if (line.contains(searchString)) { //, Qt::CaseSensitive
//            // do something
//            QRegExp rx("[:]");// match a comma or a space
//            list = line.split(rx);
//            nums.append(list.at(1).toLatin1());
//        }
//    } while (!line.isNull());

}

void MainWindow::on_serverlist_itemActivated(QListWidgetItem *item)
{

}
