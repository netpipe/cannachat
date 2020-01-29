#ifndef SERVER_H
#define SERVER_H
#include <QtNetwork>
#include "mainwindow.h"
class MainWindow;
class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(MainWindow *w,QObject *parent = 0);
    MainWindow *window;
protected:
    void incomingConnection(int desc);
private slots:
    void readRead();
    void disconnect();
private:
    QSet<QTcpSocket*> clients;
};

#endif // SERVER_H
