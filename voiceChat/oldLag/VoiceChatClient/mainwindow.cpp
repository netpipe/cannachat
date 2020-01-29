#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setChannelCount(2);
    format.setCodec("audio/pcm");
    format.setSampleRate(44100);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);
    sizetobuffer = format.channelCount() * (format.sampleSize() / 8) * format.sampleRate() / (double)1000 * 100;
    level = 0;
    volume = 100;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    socket = new QTcpSocket();
    socket->connectToHost("192.168.1.2",quint16(5002));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    input = new AudioInput();
    input->start(QAudioDeviceInfo::defaultInputDevice(),16,44100,2,QAudioFormat::SignedInt,QAudioFormat::LittleEndian);
    connect(input,SIGNAL(dataReady(QByteArray)),this,SLOT(SendData(QByteArray)));
    output = new QAudioOutput(format,this);
    output->setBufferSize(16384);
    device = output->start();
    // init
    issigned = true;
    readcalled = false;
    bufferrequested = true;
    time.start();
}

void MainWindow::SendData(QByteArray arr)
{
    socket->write(arr);
    socket->flush();
}

void MainWindow::readyRead()
{
    while (socket->bytesAvailable() > 0){
        buffer.append(socket->readAll());
    }
    if (!readcalled){
        readcalled = true;
        QMetaObject::invokeMethod(this,"read",Qt::QueuedConnection);
    }
}

void MainWindow::read()
{
    readcalled = false;
    if (buffer.isEmpty()){
        bufferrequested = true;
        return;
    }
    else if (buffer.size() < sizetobuffer){
        if (bufferrequested)
            return;
    }
    else {
        bufferrequested = false;
    }
    while (!buffer.isEmpty()){
        int readlen = output->periodSize();
        int chunks = output->bytesFree() / readlen;

        while (chunks){
            QByteArray middle = buffer.mid(0, readlen);
            int len = middle.size();
            buffer.remove(0, len);

            int min = std::numeric_limits<qint16>::min();
            int max = std::numeric_limits<qint16>::max();

            if (issigned)
            {
                qint16 *samples = (qint16*)middle.data();
                int size = len / sizeof(qint16);

                for (int i = 0; i < size; i++)
                {
                    int sample = (qint16)samples[i];
                    level = qMax(level, qAbs(sample) / (double)max);
                    sample = sample * (volume / (double)100);
                    sample = qBound(min, sample, max);
                    samples[i] = sample;
                }
            }
            else
            {
                quint16 *samples = (quint16*)middle.data();
                int size = len / sizeof(quint16);

                for (int i = 0; i < size; i++)
                {
                    int sample = (qint16)samples[i];
                    level = qMax(level, qAbs(sample) / (double)max);
                    sample = sample * (volume / (double)100);
                    sample = qBound(min, sample, max);
                    samples[i] = sample;
                }
            }

            if (len){
                device->write(middle);
            }
            if (time.elapsed() >= 50){
                level = 0;
                time.restart();
                if (!readcalled){
                    readcalled = true;
                    QMetaObject::invokeMethod(this,"read",Qt::QueuedConnection);
                }
                return;
            }
            chunks--;
        }

    }

}
