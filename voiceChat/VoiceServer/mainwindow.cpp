#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAudioFormat>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   /* QAudioFormat format;
    format.setSampleRate(128000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format))
        format = info.nearestFormat(format);
    output = new QAudioOutput(format);
    device = output->start();*/

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    serv = new Server(this);
    bool suc = serv->listen(QHostAddress::Any,quint16(5002));
    if (suc){
        ui->plainTextEdit->setPlainText("Started on port 5002");
    }
}

void MainWindow::setToPlain(QString s)
{
    ui->plainTextEdit->appendPlainText(s);
}
