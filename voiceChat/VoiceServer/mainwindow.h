#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QAudioOutput>
#include "server.h"
class Server;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setToPlain(QString s);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Server *serv;
};

#endif // MAINWINDOW_H
