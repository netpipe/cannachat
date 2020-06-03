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

    ~MainWindow();

    vector<IrcClient*> serverarray;

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

private:

//IrcClient *ircwidget;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
