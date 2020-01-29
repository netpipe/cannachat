#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioOutput>
#include <QAudioInput>
#include <QIODevice>
#include <QTcpSocket>
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
    void testSound(QByteArray arr);
    void SetPlainText(QString s);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QAudioInput *input;
    QAudioOutput *output;
    QIODevice *device;
    QByteArray data;
    int sizetobuffer;
    double level;
    int volume;
    Server *server;
};

#endif // MAINWINDOW_H
