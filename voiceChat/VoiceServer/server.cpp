#include "server.h"

Server::Server(MainWindow *w, QObject *parent) :
    QTcpServer(parent),
    window(w)
{

}

void Server::incomingConnection(int desc)
{
    QTcpSocket *socket = new QTcpSocket();
    socket->setSocketDescriptor(desc);
    clients.insert(socket);
    connect(socket,SIGNAL(readyRead()),this,SLOT(readRead()));
    window->setToPlain("CLient connected "+QString::number(socket->socketDescriptor()));
}

void Server::readRead()
{
    QTcpSocket *socket = (QTcpSocket*)sender();
    while (socket->bytesAvailable() > 0){
        QByteArray arr = socket->readAll();
        for (auto p : clients){
            if (p != socket){
                p->write(arr.data(),arr.size());
            }
        }
    }
}

void Server::disconnect()
{
    QTcpSocket *socket = (QTcpSocket*)sender();
    clients.remove(socket);
    window->setToPlain("client disccont");
}
