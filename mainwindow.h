#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QPlainTextEdit>
#include <QThread>
#include "ircclient.h"
#include <QVector>
#include <vector>
#include <QListWidgetItem>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#ifdef SOUND
#include <QMediaPlayer>
#endif
//#include "src/encryption/rsa/Rsa.h"
#include <QEvent>
#include <QThread>
#include <QDebug>
#include <QCryptographicHash>
#include <QGraphicsView>
#include <QTabWidget>
#include "src/downloadmanager.h"

#define MEDIAPLAYER

#ifdef MEDIAPLAYER
#include <QMouseEvent>
#include <QStringListModel>
#include <QDropEvent>
#include <QMimeData>

//#include "audiothread.h"
#include "src/mpqt/scanner.h"
#include "src/mpqt/track.h"
#include "src/mpqt/trackmodel.h"
#include "src/mpqt/trackdelegate.h"
#endif



using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    void importThemeInfoFromFile();
    void loadThemeFile(QString path, QString name);

    //      virtual void resizeEvent(QResizeEvent *event);
    MainWindow(QWidget *parent = 0);
bool loaded=false;
    QString fileName;
    void resizeEvent(QResizeEvent* event);
void themeInit();
void writesettings();
void readsettings();
void on_smtptestmessage_clicked();
QString GetRandomString(int length,QString chars);
QString GetReallyRandomString(int length,QString chars);
void createEmailTable();
void EmailInsertWallet();
void getEmailSettings();
    ~MainWindow();
#ifdef SOUND
    QMediaPlayer*player;
#endif
    vector<IrcClient*> serverarray;
void playsound(QString test);

    void unCompress(QString filename , QString ofilename);
    void Compress(QString filename , QString ofilename);
int adminftp=0;

    QString decodetxQR();
    int smtpsend(QString toemail,QString Message);
    #if DOWNLOAD
        void Download(QString URL);
        DownloadManager manager;
    #endif
        //encryption
        QString encryptxor(QString test,QString key);
        QString decryptxor(QString string,QString key);

        QByteArray md5Checksum(QString stuff);
        QByteArray fileChecksum(const QString &fileName,QCryptographicHash::Algorithm hashAlgorithm);
        QString rot13( const QString & input );

        QString simplecrypt(QString string,QString key,QCryptographicHash::Algorithm hash);
        QString simpledecrypt(QString string,QString key,QCryptographicHash::Algorithm hash);
#ifdef ENCRYPTION
    Rsa *rsaTester;
    BigInt m_e, m_n;
    QString aesKey;

#endif
    QString encdec(QString ,int );
    QString encdec2(QString ,int );
    #ifdef ENCRYPTION
    QString rsaenc(QString input, Rsa *rsa = NULL);
    QString rsadec(QString input, Rsa *rsa);
    #endif
    QByteArray aesenc(QString input,QString,QString);
    QString aesdec(QByteArray input,QString,QString);

    QByteArray EncryptMsg(QString plainMsg,QString aeskey1,QString aeskey2);
    #ifdef ENCRYPTION
    QString DecryptMsg(QByteArray encryptedMsg, Rsa *rsa,QString aeskey1,QString aeskey2);
#endif


    void GenerateQRCode(QString data);
    void EAN13(QString productname,QString country,QString ean,QGraphicsView *graphicsView);
    QString decodeqr(QString image);
void QRCode(QString text2);


#ifdef MEDIAPLAYER
    TrackModel *trackModel;
  //  AudioThread *audio;
    Scanner *scanner;

    QList<Track *> tracklist;
    QPoint dragPos;
    bool moving,shuffle;
    short repeatMode;
    long position;
    QAction *clearAction,*scanAction;

protected:
    void addItem(QString s);
    void dropEvent(QDropEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
    void setRepeatModeIcon();
    void nextTrack(bool next);
    bool hasNextTrack();

public slots:
    void onStartOfPlayback(double total);
    void onEndOfPlayback();
    void onPauseOfPlayback();
    void onCurPos(double position, double total);
    void onFileAdded(QString file);
    void onClearList();
    void onFindMusic();
private slots:
    void on_closeButton_clicked();
    void on_horizontalSlider_sliderPressed();
    void on_horizontalSlider_sliderReleased();
    void on_pushButton_play_clicked();
    void on_pushButton_repeat_clicked();
    void on_pushButton_shuffle_clicked();
    void on_pushButton_prev_clicked();
    void on_pushButton_next_clicked();

    void on_listView_clicked(const QModelIndex &index);
    void showContextMenuForWidget(const QPoint &pos);
    void stopScanner();
#endif
private slots:
    void on_actionExit_triggered();
    
    void on_actionNew_triggered();
    
    void on_actionClose_triggered();
    
    void on_actionRestore_triggered();

    void on_themeApply_clicked();

    void on_scantheme_clicked();

    void on_cmbTheme_currentIndexChanged(const QString &arg1);

    void on_connect_clicked();

    void on_actionSave_triggered();

    void on_addserver_clicked();

    void on_removeserver_clicked();

    void on_addchannel_clicked();

    void on_removechannel_clicked();

    void on_serveredit_clicked();

    void on_editchannel_clicked();

    void on_joinchannel_clicked();

    void on_channelList_clicked(const QModelIndex &index);

    void on_serverlist_clicked(const QModelIndex &index);

    void on_serverlist_currentRowChanged(int currentRow);

    void on_ftpserver_clicked();
    void on_smtpsave_clicked();
    void on_scan_clicked();

    void on_generate_clicked();

    void on_savetofile_clicked();

private:
    QSqlDatabase db;
//IrcClient *ircwidget;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
