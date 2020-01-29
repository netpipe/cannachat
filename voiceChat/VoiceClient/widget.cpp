#include "widget.h"
#include "ui_widget.h"
#include <QMediaPlayer>
#include <QFile>
#include <thread>
#include <chrono>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    socket = new QTcpSocket(this);
    socket->connectToHost(ui->lineEdit->text(),quint16(5002));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error()));

    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    //If format isn't supported find the nearest supported
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(format))
        format = info.nearestFormat(format);
    ui->label_2->setText(info.deviceName()); // the supported Input device


    QAudioFormat format2;
    format2.setSampleRate(44100);
    format2.setChannelCount(2);
    format2.setSampleSize(16);
    format2.setCodec("audio/pcm");
    format2.setByteOrder(QAudioFormat::LittleEndian);
    format2.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info2(QAudioDeviceInfo::defaultOutputDevice());
    if (!info2.isFormatSupported(format2))
        format2 = info2.nearestFormat(format2);
    ui->label->setText(info2.deviceName()); // the supported output device

    input = new QAudioInput(format);
    input->setBufferSize(16384);

    output = new QAudioOutput(format2);
    output->setBufferSize(16384);

    device = output->start();

    input->start(socket);

    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
}

void Widget::error()
{
    ui->label->setText(socket->errorString());
}

void Widget::readyRead()
{
    while (socket->bytesAvailable() > 0){
        QByteArray arr = socket->readAll();
        device->write(arr.data(),arr.size());
    }
}

