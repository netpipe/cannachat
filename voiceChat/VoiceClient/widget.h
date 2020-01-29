#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>
#include <QAudioInput>
#include <QAudioOutput>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();
    void error();
    void readyRead();

private:
    Ui::Widget *ui;
    QAudioInput *input;
    QTcpSocket *socket;
    QAudioOutput *output;
    QIODevice *device;
};

#endif // WIDGET_H
