#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QPlainTextEdit>
#include <QThread>
#include "ircclient.h"
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

    void resizeEvent(QResizeEvent* event);


    ~MainWindow();
private slots:
    void on_actionExit_triggered();
    
    void on_actionNew_triggered();
    
    void on_actionClose_triggered();
    
    void on_actionRestore_triggered();

    
private:
    IrcClient *ircwidget;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
