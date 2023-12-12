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

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QClipboard>
#include <QStandardItemModel>
//#include <QtCore>
#include <src/coingenerator.h>

#include "src/encryption/encryption.h"
#include "src/encryption/rsa/Rsa.h"
#include "src/downloadmanager.h"
#include "src/email.h"
#include "src/ftp-server/ftpgui.h"
//#include <src/oglwidget.h>


#ifdef BARCODE
    #include "src/QRCode/QrCode.hpp"
    #include <QGraphicsSvgItem>
    #include <QGraphicsView>
    #include "src/Barcode/quirc/tests/inspect.h"
    using qrcodegen::QrCode;
    #include "src/Barcode/functii.h"
#endif

#include <QGraphicsSvgItem>
#include <QFileDialog>
#include <fstream>
#include <QDirIterator>
#ifdef QUAZIP
#include "src/quazip/quazip.h"
#include "src/quazip/quazipfile.h"
#include "src/quazip/JlCompress.h"
#endif

#ifdef MEDIAPLAYER
#include <src/player.h>
#endif

#ifdef FTP
    FTPGUI *ftpgui;
    #include "src/ftp-server/CSslSocket/csslsocket.h"
#endif

#ifdef PYTHON
#include <src/python/qtpython.h>
#endif



#include <src/figlet/figlet.h>

//    extern "C" {
//    int figlet(int argc, char *argv);
//    }

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
       ui->setupUi(this);

      this->setWindowTitle("Cannachat");

       //salmon color
  //    Dialog1 *newDlg = new Dialog1();
  //    this->hide();
  //    int result = newDlg->exec();
  //    this->show();
  //    delete newDlg;

  //     ServerDlg *settings = new ServerDlg();
      // settings->show();

 //      connect(settings, SIGNAL(findtext()), this , SLOT(on_search_clicked()));

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

     if (ui->nickname->text().toLatin1() == "") {
         qDebug() << "setnickname";
         ui->nickname->setText("guest" + QString::number( qrand() % 9999) );
        }

     ui->serverlist->setCurrentRow(0);

themeInit();
readsettings();

#ifdef MEDIAPLAYER
ui->pushButton_play->setIcon(QIcon("./Resource/img/btn_play.png"));
ui->pushButton_play->setIcon(QIcon("./Resource/img/btn_play.png"));
ui->label_3->setPixmap(QPixmap("./Resource/img/freemp.png"));
ui->pushButton_shuffle->setIcon(QIcon("./Resource/img/btn_shuffle_off.png"));
ui->pushButton_prev->setIcon(QIcon("./Resource/img/btn_previous.png"));
ui->pushButton_next->setIcon(QIcon("./Resource/img/btn_next.png"));
ui->pushButton_repeat->setIcon(QIcon("./Resource/img/btn_repeat.png"));
//repeatMode = 0;
repeatMode = 0;
moving = false;
setRepeatModeIcon();
shuffle = false;
position = 0;
ui->listView->setEditTriggers(0);
ui->listView->setAttribute(Qt::WA_MacShowFocusRect, false);
ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
connect(ui->listView,SIGNAL(customContextMenuRequested(const QPoint &)),
        SLOT(showContextMenuForWidget(const QPoint &)));

//ui->horizontalSlider_2->setMaximum(0);
//ui->horizontalSlider_2->setSliderDown(true);
trackModel = new TrackModel;
trackModel->list = tracklist;

ui->listView->setModel(trackModel);
ui->listView->setItemDelegate(new TrackDelegate);
setAcceptDrops(true);
scanner = NULL;

//audiothread
//Added By Rajiv
audio = new AudioThread(this);
//connect(audio, SIGNAL(startOfPlayback(double )), this, SLOT(onStartOfPlayback(double)));
connect(audio, SIGNAL(startOfPlayback()), this, SLOT(onStartOfPlayback()));
connect(audio, SIGNAL(endOfPlayback()), this, SLOT(onEndOfPlayback()));
connect(audio, SIGNAL(pauseOfPlayback()), this, SLOT(onPauseOfPlayback()));
connect(audio, SIGNAL(curPos(double, double)), this, SLOT(onCurPos(double, double)));
connect(audio, SIGNAL(sliderTime(QString,qint64,qint64)), this, SLOT(onSlidertime(QString,qint64,qint64)));
//connect(ui->horizontalSlider_2, &QSlider::sliderMoved, this,SLOT(seek(int)));
connect(ui->horizontalSlider_2, &QSlider::sliderMoved, this,&MainWindow::seek);
//connect(ui->horizontalSlider_2, SIGNAL(sliderPressed), this, SLOT(on_horizontalSlider_sliderPressed()));

clearAction = new QAction(tr("Clear"), this);
connect(clearAction, SIGNAL(triggered()), this, SLOT(onClearList()));
//scanAction = new QAction(tr("Scan home dir: "+(QDir::home()).absolutePath().toUtf8()), this);
//connect(scanAction, SIGNAL(triggered()), this, SLOT(onFindMusic()));

//QMenuBar *m_menuBar = new QMenuBar(this);
//QMenu *playlistMenu = new QMenu(QString::fromUtf8("Playlist"));
//playlistMenu->addAction(clearAction);
//playlistMenu->addAction(scanAction);
//m_menuBar->addAction(playlistMenu->menuAction());

  audio->setVideoOutput(ui->vidwid);
dlmanager = new DownloadManager;
#endif

#ifdef IRRLICHT
    //this->installEventFilter(this);

   //   ui->irrRenderWidget0->grabKeyboard();
#endif
//this->removeEventFilter(this);

#ifdef FTP
  CSslSocket* ssl_socket = new CSslSocket();
  if(ssl_socket)
  {
      delete ssl_socket;
  }
#endif
}



bool MainWindow::eventFilter( QObject *o, QEvent *e )
{
    #ifdef IRRLICHT
    if ( e->type() == QEvent::KeyPress ) {
        // special processing for key press
        QKeyEvent *k = (QKeyEvent *)e;
         this->widget->keyPressEvent(k);

    //if (k->key()==69){this->removeEventFilter(this);} //key e

  //  if (k->key()==Qt::Key_Escape){this->removeEventFilter(this);  ui->irrRenderWidget0->releaseMouse();ui->irrRenderWidget0->releaseKeyboard();}
   // if (k->key()==Qt::Key_Escape){this->removeEventFilter(this);  this->widget->releaseMouse();this->widget->releaseKeyboard();}
   if (k->key()==Qt::Key_Escape){this->removeEventFilter(this);  this->widget->releaseMouse();ui->irrRenderWidget0->releaseKeyboard();}

     //   qDebug( "Ate key press %d", k->key() );
        return 1; // eat event
    }else if ( e->type() == QEvent::KeyRelease) {
         QKeyEvent *k = (QKeyEvent *)e;
        this->widget->keyReleaseEvent(k);

      return 1;
    } else if ( e->type() == QEvent::MouseButtonPress) {
        QMouseEvent* ee =(QMouseEvent*)e;
        this->widget->mousePressEvent(ee);
            return 1;
     } else if ( e->type() == QEvent::MouseButtonRelease) {
           QMouseEvent* ee =(QMouseEvent*)e;
           this->widget->mouseReleaseEvent(ee);
               return 1;
        }  else if ( e->type() == QEvent::MouseMove) {
               QMouseEvent* ee =(QMouseEvent*)e;
               this->widget->mouseMoveEvent(ee);
                //  qDebug() << "mousemove" ;
                   return 1;
            }
    else {
        // standard event processing
        return 0;
    }
#endif
}


MainWindow::~MainWindow()
{
//   ui->centralWidget->findChild<QWidget *>("irrRenderWidget0"), false);
//    this->irr0->init();
//    this->irr1 = new IrrCoreRenderer(ui->centralWidget->findChild<QWidget *>("irrRenderWidget1"), 1);
//    this->irr1->init();
    delete ui;
    #ifdef MEDIAPLAYER
    delete player;
    delete audio;
    #endif

#ifdef IRRLICHT
    delete widget;
#endif
//QApplication::exit();
}

void MainWindow::QRCode(QString text2) {
#ifdef BARCODE

    QString maxqrstr;
    for (int i=0 ;i < 2000 ; i++){ //4296 / 8 per ascii  537
        maxqrstr.append("i");
    }

 //   qDebug () << maxqrstr.toUtf8().size();
 //       qDebug () << maxqrstr.toLatin1().size();
         //   qDebug () << maxqrstr.toWCharArray().size();

std::wstring text ( text2.toStdWString() );

//char *text2 = text.c_str();
const wchar_t* wstr = text.c_str() ;
    char mbstr[4000];
    std::wcstombs(mbstr, wstr, 4000);

    const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;  // Error correction level

    const QrCode qr = QrCode::encodeText( mbstr , errCorLvl);

    ofstream write;

    std::string   filename = "tmp.svg";
    write.open(filename.c_str(), ios::out | ios::binary);
    write << qr.toSvgString(4);

    QImage *img_object = new QImage();
    img_object->load("./tmp.svg");
    QPixmap image = QPixmap::fromImage(*img_object);
 //   QPixmap scaled_img = image.scaled(this->width(), this->height(), Qt::KeepAspectRatio);
    QPixmap scaled_img = image.scaled(ui->graphicsView->width(), ui->graphicsView->height(), Qt::KeepAspectRatio);
    QGraphicsScene *scene= new QGraphicsScene();
   // scene->addItem(new QGraphicsSvgItem("./tmp.svg"));
    scene->addPixmap(scaled_img);
    scene->setSceneRect(scaled_img.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
#endif
}

void MainWindow::EAN13(QString productname,QString country,QString ean,QGraphicsView *graphicsView){ //barcode not used
#ifdef BARCODE
//    std::string code13 = EAN13::appendChecksum("123", "123456789"); //countrycode 3 letters,European Article Number 9 digits no spaces
//    std::string svg = EAN13::createSvg("productName test", code13);

    std::string code13 = EAN13::appendChecksum(country.toLatin1(), ean.toLatin1()); //countrycode 3 letters,European Article Number 9 digits no spaces
    std::string svg = EAN13::createSvg(productname.toStdString(), code13);

    ofstream write;
    std::string   filename = "tmp.svg";
    write.open(filename.c_str(), ios::out | ios::binary);
    write << svg.c_str();

    QImage *img_object = new QImage();
    img_object->load("./tmp.svg");
    QPixmap image = QPixmap::fromImage(*img_object);
 //   QPixmap scaled_img = image.scaled(this->width(), this->height(), Qt::KeepAspectRatio);
    QPixmap scaled_img = image.scaled(graphicsView->width(), graphicsView->height(), Qt::KeepAspectRatio);
    QGraphicsScene *scene= new QGraphicsScene();
   // scene->addItem(new QGraphicsSvgItem("./tmp.svg"));
    scene->addPixmap(scaled_img);
    scene->setSceneRect(scaled_img.rect());
    graphicsView->setScene(scene);
    graphicsView->show();
#endif
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
 //  ircwidget[0].resize(ui->chatwidget->width(),ui->chatwidget->height());
#ifdef IRRLICHT
   widget->resizeIrrWidget(0, 0, this->size().width(), this->size().height());
  // this->irr1->resizeIrrWidget(this->size().width()/2, 0, this->size().width()/2, this->size().height());
#endif
   // Your code here.
}
#ifdef MEDIAPLAYER
void MainWindow::playsound(QString test){

    // player->setMedia(QUrl("qrc:/sounds/ec1_mono.ogg"));
    // player->setMedia(QUrl::fromLocalFile("./paddle_hit.wav"));
     //or play from memory
      QFile file(test.toLatin1());
      file.open(QIODevice::ReadOnly);
      QByteArray* arr = new QByteArray(file.readAll());
      file.close();
      QBuffer* buffer = new QBuffer(arr);
      buffer->open(QIODevice::ReadOnly);
      buffer->seek(0);
//qDebug() << "Media supported state -> " << QMediaPlayer::hasSupport("video/mp4"); // this gives a "1"
      player->setVolume(10);
 //    media->setMedia("sound.mp3");
     player->setMedia(QMediaContent(), buffer);
     player->play();
}
#endif
void MainWindow::readsettings(){
    bool settingsexists=false;
QFile Fout("settings.txt");    if(Fout.exists())    {       settingsexists=true;    }    Fout.close();
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

    ui->nickname->setText(nums.at(0).toLatin1());
    ui->password->setText(nums.at(1).toLatin1());
    ui->identname->setText(nums.at(2).toLatin1());
    ui->client->setText(nums.at(3).toLatin1());
    ui->quitmsg->setText(nums.at(4).toLatin1());
    ui->idlemessage->setText(nums.at(5).toLatin1());
    ui->awaymessage->setText(nums.at(6).toLatin1());

    ui->colors->setChecked(nums.at(7).toInt());
    ui->themes->setChecked(nums.at(8).toInt());
    ui->smtp->setChecked(nums.at(9).toInt());
    ui->logging->setChecked(nums.at(10).toInt());
    ui->identify->setChecked(nums.at(11).toInt());
    nums.clear();

    bool serversexists=false;
QFile Fout("servers.txt");    if(Fout.exists())    {       serversexists=true;    }    Fout.close();
if (serversexists){
qDebug() << "test";
    QFile MyFile2("servers.txt");
    MyFile2.open(QIODevice::ReadWrite);
    QTextStream in2 (&MyFile2);

    do {
        line = in2.readLine();
        searchString=":";
        if (line.contains(searchString)) { //, Qt::CaseSensitive
            // do something
           // QRegExp rx("[:]");// match a comma or a space
          //  list = line.split(rx);
           nums.append(line);
        }
    } while (!line.isNull());
    MyFile2.close();
// ui->serverlist->clearSelection();
//    ui->serverlist->blockSignals(true);
//    ui->serverlist->clear();
//    ui->serverlist->blockSignals(false);
    for (int i=0; i <= ui->serverlist->count(); i++){
//        ui->serverlist->takeItem(ui->serverlist->currentRow());
//    }

   // while(ui->serverlist->count()>0)    {
  //      ui->serverlist->setCurrentRow( 0 );
  //      ui->serverlist->itemActivated(0);
 //       ui->serverlist->activated(0);
      ui->serverlist->takeItem(ui->serverlist->currentRow());
          ui->serverlist->setCurrentRow( ui->serverlist->count() - 1 );
    }
   // ui->serverlist->currentRow())->setText(nums.at(0).toLatin1()
   // ui->serverlist->setCurrentRow(ui->serverlist->count() - 1);
   //     ui->serverlist->takeItem(0);
 //ui->serverlist->clearSelection();
   // qDeleteAll(ui->serverlist->selectedItems());

                  //      int sized = ui->channelList->count();
    ui->serverlist->item(ui->serverlist->currentRow())->setText(nums.at(0).toLatin1());
    for (int i=1; i < nums.count() ; i++){
        ui->serverlist->addItem(nums.at(i).toLatin1());
    }
}


}
    //ircwidget->resize(ui->chatwidget->width(),ui->chatwidget->height());
       writesettings();
}

void MainWindow::writesettings(){

 //   bsaving=1;
    QFile file("settings.txt");

          if(file.open(QIODevice::WriteOnly | QIODevice::Text))
          {
                  file.seek(0);
              QTextStream stream(&file);
                stream << "name:" << ui->nickname->text().toLatin1() << endl;
                stream << "password:" << ui->password->text().toLatin1()<< endl;
                stream << "ident:" << ui->identname->text().toLatin1()<< endl;
                stream << "client:" << ui->client->text().toLatin1()<< endl;
                stream << "quit:" << ui->quitmsg->text().toLatin1()<< endl;
                stream << "idle:" << ui->idlemessage->text().toLatin1()<< endl;
                stream << "away:" << ui->awaymessage->text().toLatin1()<< endl;

                stream << "colors:" << ui->colors->isChecked()<< endl;
                stream << "themes:" << ui->themes->isChecked()<< endl;
                stream << "smtp:" << ui->smtp->isChecked()<< endl;
                stream << "logging:" << ui->logging->isChecked()<< endl;
                stream << "identify:" << ui->identify->isChecked()<< endl;

              file.close();
          }

          QStringList splitlist = ui->serverlist->currentItem()->text().split(":");
          QString servername = splitlist[0].toUtf8();

          QFile file2(servername.toLatin1()+".txt");
                if(file2.open(QIODevice::WriteOnly | QIODevice::Text))
                {
                    file2.seek(0);
                    QTextStream stream(&file2);
                    int sized = ui->channelList->count();
                    for (int i=0; i < sized; i++){

                        ui->channelList->setCurrentRow(i);
                         stream << servername.toLatin1()+":" << ui->channelList->currentItem()->text().toLatin1() <<  ":" << ui->autojoinchk->isChecked()<< ":" << ui->chanpass->text() <<endl;
                    }
                    file2.close();
                }

qDebug() << "writing settings";
          QFile file3("servers.txt");
                if(file3.open(QIODevice::WriteOnly | QIODevice::Text))
                {

                    QTextStream stream(&file3);
                                        file3.seek(0);
                    int sized = ui->serverlist->count();
                    qDebug() << sized;
                    for (int i=0; i < sized; i++){
                        ui->serverlist->setCurrentRow(i);

                         stream << ui->serverlist->currentItem()->text().toLatin1() << endl;
                    }
                    file3.close();
                }
//bsaving=0;
}

void MainWindow::on_actionExit_triggered()
{
QApplication::quit();
}

void MainWindow::on_actionNew_triggered()
{
//    serversw = new ServerDlg();
//    serversw->show();
}

void MainWindow::on_actionClose_triggered()
{

}

void MainWindow::on_actionRestore_triggered()
{

}

void MainWindow::on_connect_clicked()
{
    QStringList splitlist = ui->serverlist->currentItem()->text().split(":");
    QString servername = splitlist[0].toUtf8();
    qDebug() << splitlist[0].toUtf8();
   // QString channel =  "#cannachat";
    int port = splitlist[1].toInt();
    bool ssl = splitlist[2].toInt();
    QString password = "";
//    if (ui->chkpassword->isChecked()){
//        if (splitlist.count() > 3){
        password = splitlist[3].toUtf8();
//        }
//        else {password = "";}
//    }else{
//        password = "";
//    }


//bconnecting = 1;
//QFile Fout(servername.toLatin1()+".txt");    if(Fout.exists())    {     //if(!bsaving) {ui->channelList->clear();}
//ui->channelList->clear();    }    Fout.close();
    // Autojoin the channel
    QString channel = ui->channeledit->text().toLatin1();
    QStringList splitlist2 = channel.split(":");
    QString servername2 = splitlist2[0].toUtf8();

    QString searchString(":");
    QFile MyFile(servername.toLatin1()+".txt");
    MyFile.open(QIODevice::ReadWrite);
    QTextStream in (&MyFile);

    QStringList list;        QString line;        QStringList nums;

    do {
        line = in.readLine();
        if (line.contains(searchString.toLatin1())) { //, Qt::CaseSensitive
            QRegExp rx("[:]");// match a comma or a space
            list = line.split(rx);
            if (list.at(2).toLatin1() == "1"){ui->autojoinchk->setChecked(1);
                //   serverarray[0]->appendText("/PRIVMSG #communi :testing123\r\n");
               // QString //test = "/JOIN ";
                    QString     test = list.at(1).toLatin1();
                         test += " ";
                        test += list.at(3).toLatin1();
                        test +=  "\r\n";
                        //nums << list.at(1).toLatin1();//test.toLatin1()
                        nums << test.toLatin1();
               // CilentAutoChannelConnect->appendText(test.toLatin1());
                //CilentAutoChannelConnect->JoinChannel(list.at(1).toLatin1());
//qDebug() << "autojoin " << list.at(1).toLatin1() ;
            }
            if (list.at(3).toLatin1() != ""){ui->chanpass->setText(list.at(3).toLatin1());}
        }
    } while (!line.isNull());
    MyFile.close();


   // if(channel.length()>0)
   // {
     //   channel = channel;
        IrcClient *CilentAutoChannelConnect = new IrcClient(ui->tabWidget->findChild<QWidget *>("chatwidget"), ui->nickname->text().toUtf8(), nums, servername.toUtf8(),port,ssl,password.toUtf8());
        serverarray.push_back(CilentAutoChannelConnect);

      // CilentAutoChannelConnect->JoinChannel(channel);
     //  on_joinchannel_clicked();
        //serverarray.push_back(new IrcClient( ui->tabWidget->findChild<QWidget *>("chatwidget"), ui->nickname->text().toUtf8(), channel.toUtf8(), servername.toUtf8(),port,ssl,password.toUtf8()));
        qDebug() << serverarray.size();
       ui->tabWidget->addTab(serverarray[serverarray.size()-1], QString::number(serverarray.size()-1)+":"+servername.toLatin1());
     //  CilentAutoChannelConnect->appendText("/PRIVMSG #communi :testing123");
       foreach ( QString test3,nums){
       //  CilentAutoChannelConnect->JoinChannel(test3.toLatin1());
       }
       //sleep(5);
       //CilentAutoChannelConnect->appendText("/JOIN #communi\r\n");
 //   }
//bconnecting=0;
}

void MainWindow::on_actionSave_triggered()
{
    writesettings();
}

void MainWindow::on_addserver_clicked()
{
    ui->serverlist->addItem(ui->server->text().toLatin1());
    writesettings();
}

void MainWindow::on_removeserver_clicked()
{
    //check for duplicate server before removing file
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Are you sure ?", "remove ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      qDebug() << "Yes was clicked";
      if (reply){
          //    ui->serverlist->addItem(ui->server->text().toLatin1())
            qDeleteAll(ui->serverlist->selectedItems());
            writesettings();
            //delete file
            //ui->serverlist->currentItem()->text()
      }
    }


}


void MainWindow::on_addchannel_clicked()
{
    ui->channelList->addItem(ui->channeledit->text().toLatin1());
    writesettings();
}

void MainWindow::on_removechannel_clicked()
{
            //    QListWidgetItem items = ui->listWidget->selectedItems();
            //ui->listWidget->removeItemWidget(item);
//    QList<QListWidgetItem*> items0= ui->channelList->selectedItems();
//    foreach(QListWidgetItem * item, items)
//    {
//        delete ui->channelList->takeItem(ui->channelList->row(item));
//    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Are you sure ?", "remove ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      qDebug() << "Yes was clicked";
      if (reply){
        qDeleteAll(ui->channelList->selectedItems());

        writesettings();
        }
    }
}

void MainWindow::on_serveredit_clicked()
{
        ui->serverlist->item(ui->serverlist->currentRow())->setText(ui->server->text().toLatin1());
    //ui->channelList->addItem(ui->editchannel->text().toLatin1());
    writesettings();
}

void MainWindow::on_editchannel_clicked()
{
    ui->channelList->item(ui->channelList->currentRow())->setText(ui->channeledit->text().toLatin1());
    //ui->channelList->addItem(ui->editchannel->text().toLatin1());
    writesettings();
}

void MainWindow::on_joinchannel_clicked()
{
    //get selected tab
serverarray[0]->JoinChannel(ui->channelList->currentItem()->text().toLatin1());

}

void MainWindow::on_channelList_clicked(const QModelIndex &index)
{
    ui->channeledit->setText(ui->channelList->currentItem()->text().toLatin1());
}

void MainWindow::on_serverlist_clicked(const QModelIndex &index)
{


  ui->server->setText(ui->serverlist->currentItem()->text().toLatin1());
}

void MainWindow::on_serverlist_currentRowChanged(int currentRow)
{
    //load channels for server maybe use sqlite db
    QStringList splitlist = ui->serverlist->currentItem()->text().split(":");
    QString servername = splitlist[0].toUtf8();
    QFile Fout(servername.toLatin1()+".txt");    if(Fout.exists())    {     //if(!bsaving) {ui->channelList->clear();}
    ui->channelList->clear();    }    Fout.close();
    QString password;
//   if( splitlist.count() > 3 ){
//     password = splitlist[3].toUtf8();
//     if (password == ""){
//             ui->chkpassword->setChecked(0);
//     }else{
//    ui->chkpassword->setChecked(1);
//            }
//   }
ui->autojoinchk->setChecked(0);
   // qDebug() << "channels";
    QString searchString(":");
    QFile MyFile(servername.toLatin1()+".txt");
    MyFile.open(QIODevice::ReadWrite);
    QTextStream in (&MyFile);

    QStringList list;
    QString line;
    QStringList nums;

    do {
        line = in.readLine();
        if (line.contains(searchString.toLatin1())) { //, Qt::CaseSensitive
            QRegExp rx("[:]");// match a comma or a space
            list = line.split(rx);
            if (list.at(2).toLatin1() == "1"){ui->autojoinchk->setEnabled(1);}
            if (list.at(3).toLatin1() != ""){ui->chanpass->setText(list.at(3).toLatin1());}
           // nums.append(list.at(1).toLatin1());
            QString test = list.at(1).toLatin1();
            test +=":";
                   test +=list.at(2).toLatin1();
                     test +=":";
                     test +=list.at(3).toLatin1();
            nums.append(test);
        }
    } while (!line.isNull());
    MyFile.close();
    foreach (QString list2,nums){
        ui->channelList->addItem(list2.toLatin1());
    }
}

void MainWindow::on_ftpserver_clicked()
{
#ifdef FTP
    if (adminftp==0){
    ftpgui = new FTPGUI;
    adminftp=1;
    }
    if (adminftp) { ftpgui->show();}
#endif
}



//void MainWindow::clientusbtxrx(){
//    //import db's and overwrite if valid md5sums after copying yearly dbs and md5sums from server
//    //applying rx file to compare?

//    QStringList list;
//    //export db's and overwrite if valid
//    QDirIterator it("./db/", QStringList() << "*.sqlite", QDir::Files, QDirIterator::Subdirectories);
//    while (it.hasNext()){
//      //  QFileInfo fileInfo(f.fileName());
//     list << it.next().toLatin1();
//    }


//    if(JlCompress::compressFiles("saveFile.zip", list)){
////        QMessageBox Msgbox;
////            Msgbox.setText("zipped");
////            Msgbox.exec();
//    } else {
//                   QMessageBox Msgbox;
//                       Msgbox.setText("zip file not found ");
//                       Msgbox.exec();
//    }


//    unCompress("saveFile.zip" , "./db/");

//}

//void MainWindow::serverusbtxrx(){
//    //automatic function to do rxtx from usb for cold storage

//    //verify tx file apply
//    QStringList list;
//    //export db's and overwrite if valid
//    QDirIterator it("./db/", QStringList() << "*.sqlite", QDir::Files, QDirIterator::Subdirectories);
//    while (it.hasNext()){
//      //  QFileInfo fileInfo(f.fileName());
//     list << it.next().toLatin1();
//    }


//    if(JlCompress::compressFiles("saveFile.zip", list)){
////        QMessageBox Msgbox;
////            Msgbox.setText("zipped");
////            Msgbox.exec();
//    } else {
//                   QMessageBox Msgbox;
//                       Msgbox.setText("zip file not found ");
//                       Msgbox.exec();
//    }


//}

//void MainWindow::on_receivesaveqr_clicked()
//{
//    QString fileName= QFileDialog::getSaveFileName(this, "Save image", QCoreApplication::applicationDirPath(), "BMP Files (*.bmp);;JPEG (*.JPEG);;PNG (*.png)" );
//        if (!fileName.isNull())
//        {
//            QPixmap pixMap = this->ui->ReceiveQR->grab();
//            pixMap.save(fileName);
//        }
//}

//void MainWindow::on_sendSaveqr_clicked()
//{
//    QString fileName= QFileDialog::getSaveFileName(this, "Save image", QCoreApplication::applicationDirPath(), "BMP Files (*.bmp);;JPEG (*.JPEG);;PNG (*.png)" );
//        if (!fileName.isNull())
//        {
//            QPixmap pixMap = this->ui->sendSaveqr->grab();
//            pixMap.save(fileName);
//        }
//}

void MainWindow::on_scan_clicked()
{
#ifdef BARCODE
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open rx/tx"), "./", tr("rx/tx files (*.png *.jpg)"));
        ui->decode->setText(decodeqr(fileName));
#endif
}

void MainWindow::on_generate_clicked()
{
    QRCode(ui->inputbox->text());


}

void MainWindow::on_savetofile_clicked()
{
    QString fileName= QFileDialog::getSaveFileName(this, "Save image", QCoreApplication::applicationDirPath(), "BMP Files (*.bmp);;JPEG (*.JPEG);;PNG (*.png)" );
        if (!fileName.isNull())
        {
            QPixmap pixMap = this->ui->graphicsView->grab();

            pixMap.save(fileName);
        }
}



int figlet_wrapper(char* csv)
{
#ifdef FIGLET
std::vector<char*> parts;
char* part = strtok(csv, ",");
while (part) {
    parts.push_back(part);
    part = strtok(nullptr, ",");
}
return figlet(parts.size(), parts.data());
#endif
return 0;
}

void MainWindow::on_asciigen_clicked()
{
    QString fileslist;

     fileslist.append("blank,");

  //   fileslist.append("-f,");
   //  fileslist.append("./Resource/fig-fonts/standard.flf,");

    // QString path = QDir::temp().absoluteFilePath("standard.flf ,");//a path in the temporary folder
     //if (!(QFile::copy(":/Resource/fig-fonts/standard.flf", path)))
     //    fileslist.append(":/Resource/fig-fonts/standard.flf ,");
   //  fileslist.append(":/Resource/fig-fonts/standard.flf ,");

     //fileslist.append(ui->fonts->currentText().toLatin1() +",");
    // fileslist.append(path);
     fileslist.append(ui->asciito->text().toLatin1());


     QByteArray array = fileslist.toLocal8Bit();
     char* buffer = array.data();

     if(figlet_wrapper(buffer)) {
         qDebug() << "successful";
     }else{
           qDebug() << "returned false";
     }
     //    QString line;
     QFile file("tmpascii.txt");
     if (file.open(QIODevice::ReadOnly | QIODevice::Text)){ ui->asciifrom->setPlainText(file.readAll()); }
}


void MainWindow::on_asciito_editingFinished()
{
    QDirIterator it("./Resource/fig-fonts/", QStringList() << "*.flf", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()){
      //  QFileInfo fileInfo(f.fileName());
        ui->fonts->addItem(it.next().toLatin1());
    }
}

void MainWindow::on_pyrun_clicked()
{
#ifdef PYTHON
    qtPython();
    settext();
#endif
}

void MainWindow::on_tabWidget_currentChanged(int index)
{//switch the widgets and irrRenderer's around to do mouse picking, use qdebug on the mousemove event to see itwith qtevent mainwindow::eventFilter.

    #ifdef IRRLICHT
      //    qDebug() << "testing tabs";
    //if 3D tab
    //grab keyboard and mouse
     if (ui->tabWidget->currentIndex() == 0){
         qDebug()<< "curindex0";
          ui->irrRenderWidget0->releaseKeyboard();
                 //     ui->irrRenderWidget0->releaseMouse();
                      this->widget->releaseMouse();
                      //this->widget->releaseKeyboard();
            this->removeEventFilter(this);
            widget->bActive=0;
     }else if (ui->tabWidget->currentIndex() == 1){
             qDebug()<< "curindex1";
             this->installEventFilter(this);
            ui->irrRenderWidget0->grabKeyboard();
          //  ui->irrRenderWidget0->grabMouse();
            this->widget->grabMouse();
           // this->widget->grabKeyboard();
            widget->bActive=1;
         }else{
            ui->irrRenderWidget0->releaseKeyboard();
            this->removeEventFilter(this);
          //  ui->irrRenderWidget0->releaseMouse();
         this->widget->releaseMouse();
        // this->widget->releaseKeyboard();
            widget->bActive=0;
           qDebug()<< "releaseKeyboard";
        }
             #endif
}

void MainWindow::on_pushButton_2_clicked()
{
 //   serverarray[0]->appendText("/PRIVMSG #communi :testing123\r\n");
 serverarray[0]->appendText("/JOIN #communi\r\n");
}



void MainWindow::on_pushButton_clicked()
{
//    audio->play("https://usa19.fastcast4u.com:2000/");
#ifdef MEDIAPLAYER
audio->play("http://185.33.21.112:80/rockclassics_64",1);
//audio->play("http://136.32.49.56:8000",1);
#endif


}
#ifdef MEDIAPLAYER
void MainWindow::on_pushButton_play_clicked()
{

    if (hasNextTrack()) {
      audio->playOrPause(tracklist.at(position)->path);
      // audio->playOrPause(tracklist.at(20.0f)->path);
    }

}
#endif
//void MainWindow::on_pushButton_4_clicked()
//{
//  ///  onFindMusic("./");
//    //          QDir::homePath() + "/" +
//    //                     QStandardPaths::displayName(QStandardPaths::MusicLocation);
//}

void MainWindow::on_pushButton_3_clicked()
{
#ifdef MEDIAPLAYER
    //http://api.shoutcast.com/legacy/genresearch?k=F5NaL5p4avjtIaoy&genre=classic&limit=X,Y

QString url2 = "http://api.shoutcast.com/legacy/stationsearch?k=F5NaL5p4avjtIaoy&search=";  //ambient+beats
        url2+= ui->stationSearch->text().toLatin1();
        url2+= "&limit=1,10";

        QString fileName = "./stationsearch";
        QFile file2(fileName);
            file2.remove();
            file2.close();

   // dlmanager->Download("http://api.shoutcast.com/legacy/Top500?k=F5NaL5p4avjtIaoy");
    dlmanager->Download(url2.toLatin1());

//    QThread::sleep(3);
//qDebug() << "testing\n";
    QTime dieTime = QTime::currentTime().addMSecs( 1000 );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }

parseSearch();
#endif
}

void MainWindow::parseSearch(){
#ifdef MEDIAPLAYER
    ui->txtIds->clear();
    ui->txtNames->clear();
    ui->stationurls->clear();

QString fileName = "./stationsearch";
QFile file(fileName);
  //  file.remove();
    file.open(QIODevice::ReadOnly);
    QString s;

    QTextStream s1(&file);
    s.append(s1.readAll());


    QString inputstring = s; //ui->txtInput->toPlainText();

    QRegExp regex;
    regex.setPattern("name=\\\"([^\\\"]+)\\\"");

    QStringList namelist, idlist;
    int pos = 0;
    while (pos >= 0)
    {
        pos = regex.indexIn(inputstring, pos);

        if (pos >= 0)
        {
            QString strName = regex.cap(0);
            strName = strName.mid(5);
            namelist.push_back(strName);
            pos += strName.length();
        }
    }

    regex.setPattern("id=\\\"([0-9]+)\\\"");
    pos = 0;
    while (pos >= 0)
    {
        pos = regex.indexIn(inputstring, pos);
        if (pos >= 0)
        {
            QString strId = regex.cap(0);
            strId = strId.mid(3);
            idlist.push_back(strId);
            pos += strId.length();
        }
    }
ui->txtNames->clear();
    QString buf = "";
    int count = namelist.count();
    for (int i = 0; i < count; i ++)
    {
        buf = namelist.at(i);
          buf.remove('"');
        ui->txtNames->addItem(buf);

   //     buf += "\r\n";
    }
   // ui->txtNames->setPlainText(buf);
  //  ui->txtNames->addItem(buf);
  //  ui->txtNames->(buf);

ui->txtIds->clear();
    buf = "";
    count = idlist.count();
    for (int i = 0; i < count; i ++)
    {
        buf = idlist.at(i);
      //  buf += "\r\n";
        buf.remove('"');
           ui->txtIds->addItem(buf);

    }
  //  ui->txtIds->setPlainText(buf);

#endif
}

void MainWindow::on_chooseStationbtn_clicked()
{
    #ifdef MEDIAPLAYER
  //     QFile::remove("tunein-station.pls");
ui->stationurls->clear();
   // ui->txtIds->addItem(buf);
 //   QString url2 = "http://yp.shoutcast.com/sbin/tunein-station.pls?id=";
 //           url2 +=    ui->txtIds->currentItem()->text().toLatin1();

 //   dlmanager->Download(url2);

  //  QThread::sleep(1);

    QString filename="tunein-station.pls";
    QFile file(filename);
    QString line;
    ui->textEdit->clear();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
        while (!stream.atEnd()){
            line = stream.readLine();
            ui->textEdit->setText(ui->textEdit->toPlainText()+line+"\n");
          //  qDebug() << "linea: "<<line;
        }
    }
    file.close();


    QString inputstring = ui->textEdit->toPlainText();

    QRegExp regex;
    //regex.setPattern("name=\\\"([^\\\"]+)\\\"");

     regex.setPattern("(http:|http)//[a-zA-Z0-9./?=_%:-]*"); //https
    QStringList namelist, idlist;
    int pos = 0;
    while (pos >= 0)
    {
        pos = regex.indexIn(inputstring, pos);

        if (pos >= 0)
        {
            QString strName = regex.cap(0);
            strName = strName.mid(5);
            namelist.push_back(strName);
                ui->stationurls->addItem(strName);
            pos += strName.length();
        }
    }


    //QString fileName = "tunein-station.pls";
    //QFile file2(fileName);
    //    file2.remove();
     //   file2.close();

    //    QFile::remove("tunein-station.pls");

//audio->play("http://185.33.21.112:80/rockclassics_64",1);
  //  audio->play("http://185.33.21.112:80/rockclassics_64",1);
#endif
}



void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{

    //http://api.shoutcast.com/legacy/genresearch?k=F5NaL5p4avjtIaoy&genre=classic&limit=X,Y


    QString url2 = "http://api.shoutcast.com/legacy/stationsearch?k=F5NaL5p4avjtIaoy&search=";  //ambient+beats
            url2+= ui->stationSearch->text().toLatin1();
            url2+= "&limit=1,10";

       // dlmanager->Download("http://api.shoutcast.com/legacy/Top500?k=F5NaL5p4avjtIaoy");
      //  dlmanager->Download(url2.toLatin1());
}

void MainWindow::on_refreshbtn_clicked()
{
    #ifdef MEDIAPLAYER
    parseSearch();
#endif
}

void MainWindow::on_txtNames_currentRowChanged(int currentRow)
{
        ui->txtIds->setCurrentRow(currentRow);
}

void MainWindow::on_playstationbtn_clicked()
{
    #ifdef MEDIAPLAYER
      audio->play("http:" + ui->stationurls->currentItem()->text().toLatin1(),1);
//ui->treeWidget->currentItem()->text(0);
#endif
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
#ifdef MEDIAPLAYER
   ui->stationSearch->setText(ui->treeWidget->currentItem()->text(0) )  ;

   on_pushButton_3_clicked();

  // parseSearch();
#endif
}

void MainWindow::on_stationurls_itemDoubleClicked(QListWidgetItem *item)
{
#ifdef MEDIAPLAYER
    audio->play("http:" + ui->stationurls->currentItem()->text().toLatin1(),1);
#endif
}

void MainWindow::on_txtNames_itemDoubleClicked(QListWidgetItem *item)
{
    #ifdef MEDIAPLAYER
    QFile::remove("tunein-station.pls");
    QString url2 = "http://yp.shoutcast.com/sbin/tunein-station.pls?id=";
            url2 +=    ui->txtIds->currentItem()->text().toLatin1();

    dlmanager->Download(url2);

    QTime dieTime = QTime::currentTime().addMSecs( 1000 );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }

    on_chooseStationbtn_clicked();
#endif
}

void MainWindow::on_pushButton_4_clicked()
{

#ifdef MEDIAPLAYER
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                "/home",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);

       onFindMusic(dir.toLatin1());
#endif
}

void MainWindow::on_pushButton_5_clicked()
{
#ifdef MEDIAPLAYER
    QString fileName = QFileDialog::getOpenFileName(this, ("Open File"),
                                                    "/home",
                                                    ("Images (*.mp3 *.ogg *.wav)"));

    audio->play(fileName,0);
#endif
}
#ifdef MEDIAPLAYER
void MainWindow::onFindMusic(QString directory) {

  stopScanner();
  QString dirMusic = directory;

//          QDir::homePath() + "/" +
//                     QStandardPaths::displayName(QStandardPaths::MusicLocation);

  scanner = new Scanner(dirMusic, this);
  connect(scanner, SIGNAL(fileAdded(QString)), this,
          SLOT(onFileAdded(QString)));
  scanner->start();

}
#endif

