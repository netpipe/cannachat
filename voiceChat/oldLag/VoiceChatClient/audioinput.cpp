#include "audioinput.h"

AudioInput::AudioInput(QObject *parent) : QObject(parent)
{
}

void AudioInput::start(const QAudioDeviceInfo &devinfo,
                       int samplesize,
                       int samplerate,
                       int channels,
                       int sampletype,
                       int byteorder)
{
    format.setCodec("audio/pcm");
    format.setSampleSize(samplesize);
    format.setSampleRate(samplerate);
    format.setChannelCount(channels);
    format.setSampleType((QAudioFormat::SampleType)sampletype);
    format.setByteOrder((QAudioFormat::Endian)byteorder);

    if (!devinfo.isFormatSupported(format))
    {
        format = devinfo.nearestFormat(format);
        emit adjustSettings(format);
    }

    audio = new QAudioInput(devinfo, format, this);
    audio->setBufferSize(16384);

    device = audio->start();

    if (!device)
    {
        emit error("Failed to open audio device");
        return;
    }

    connect(device, &QIODevice::readyRead, this, &AudioInput::readyRead);
}

void AudioInput::readyRead()
{
    QByteArray data;

    //Check the number of samples in input buffer
    int len = audio->bytesReady();

    //Read sound samples from input device to buffer
    if (len)
    {
        data.resize(len);
        device->read(data.data(), len);
    }
    emit dataReady(data);
}

/*QByteArray AudioInput::header()
{
    QByteArray data;

    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << (quint8)format.sampleSize();
    stream << (quint32)format.sampleRate();
    stream << (quint8)format.channelCount();
    stream << (quint8)format.sampleType();
    stream << (quint8)format.byteOrder();

    return data;
}*/
