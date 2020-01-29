#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QAudioOutput>
#include <QAudioInput>
#include <QIODevice>
#include <QElapsedTimer>
#include "audioinput.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void SendData(QByteArray arr);

    void readyRead();

    void read();

private:
    Ui::MainWindow *ui;
    AudioInput *input;
    QTcpSocket *socket;
    QAudioFormat format;
    QByteArray buffer;
    QAudioOutput *output;
    QIODevice *device;

    int volume;
    double level;
    int sizetobuffer;
    bool bufferrequested;
    bool readcalled;
    bool issigned;
    QElapsedTimer time;
};

#endif // MAINWINDOW_H
