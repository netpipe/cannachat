#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*QAudioFormat format;
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setChannelCount(2);
    format.setCodec("audio/pcm");
    format.setSampleRate(44100);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);

    sizetobuffer = format.channelCount() * (format.sampleSize() / 8) * format.sampleRate() / (double)1000 * 100;
    output = new QAudioOutput(format, this);
    output->setBufferSize(16384);
    level = 0;
    volume = 100;

    device = output->start();*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::testSound(QByteArray arr)
{
    /*data.append(arr);
    if (data.size() < sizetobuffer) return;
    else {
        int readlen = output->periodSize();
        int chunks = output->bytesFree() / readlen;

        while (chunks){
            QByteArray middle = data.mid(0, readlen);
            int len = middle.size();
            data.remove(0, len);
            int min = std::numeric_limits<qint16>::min();
            int max = std::numeric_limits<qint16>::max();

            if (true)
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
            chunks--;
        }
    }*/
}

void MainWindow::SetPlainText(QString s)
{
    ui->plainTextEdit->appendHtml(s);
}

void MainWindow::on_pushButton_clicked()
{
    server = new Server(this);
    if (server->listen(QHostAddress::Any,quint16(ui->lineEdit->text().toInt()))){
        ui->plainTextEdit->appendHtml("<h3>Server Started...<h3>");
    }
}
