#include "server.h"

Server::Server(MainWindow *w, QObject *parent) :
    QTcpServer(parent)
{
    window = w;
}

void Server::readyRead()
{
    QTcpSocket *socket = (QTcpSocket*)sender();
    while (socket->bytesAvailable() > 0){
        QByteArray data = socket->readAll();
        for (auto &p : clients){
            if (p != socket){
                p->write(data);
                p->flush();
            }
        }
    }
   /* QByteArray data = socket->readAll();
    window->testSound(data);*/
}

void Server::disconnected()
{
    QTcpSocket *socket = (QTcpSocket*)sender();
    window->SetPlainText(" <font color=\"red\">client "+ QString::number(socket->socketDescriptor()) + " disconnected</font>");
    clients.remove(socket);
}

void Server::incomingConnection(int descriptor)
{
    QTcpSocket *socket = new QTcpSocket();
    socket->setSocketDescriptor(descriptor);
    clients.insert(socket);

    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));

   window->SetPlainText("<font color=\"green\">client " + QString::number(socket->socketDescriptor()) + " connected..</font>");
}
