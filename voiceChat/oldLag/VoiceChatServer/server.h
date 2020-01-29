#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include "mainwindow.h"

class MainWindow;
class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(MainWindow *w, QObject *parent = 0);
    MainWindow *window;
private slots:
    void readyRead();
    void disconnected();
protected:
    void incomingConnection(int descriptor);
private:
    QSet<QTcpSocket*> clients;
};

#endif // SERVER_H
