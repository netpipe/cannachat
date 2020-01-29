#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QtCore>
#include <QtMultimedia>

class AudioInput : public QObject
{
    Q_OBJECT
public:
    explicit AudioInput(QObject *parent = 0);

signals:
    void error(QString);
    void adjustSettings(QAudioFormat);
    void dataReady(QByteArray);

public slots:
    void start(const QAudioDeviceInfo &devinfo, int samplesize, int samplerate, int channels, int sampletype, int byteorder);

private slots:
    void readyRead();

private:
    QAudioInput *audio;
    QIODevice *device;
    QAudioFormat format;
};

#endif // AUDIOINPUT_H
