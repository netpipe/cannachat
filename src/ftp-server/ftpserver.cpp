#ifdef FTP
#include "ftpserver.h"

#include "sslserver.h"

#include <QDebug>
#include <QSslSocket>

FtpServer::FtpServer(QObject *parent, const QString &rootPath, int port, const QString &userName, const QString &password, bool readOnly, bool onlyOneIpAllowed,bool userslist) :
    QObject(parent)
{
    server = new SslServer(this);
    // In Qt4, QHostAddress::Any listens for IPv4 connections only, but as of
    // Qt5, it now listens on all available interfaces, and
    // QHostAddress::AnyIPv4 needs to be used if we want only IPv4 connections.
#if QT_VERSION >= 0x050000
    server->listen(QHostAddress::AnyIPv4, port);
#else
    server->listen(QHostAddress::Any, port);
#endif
    connect(server, SIGNAL(newConnection()), this, SLOT(startNewControlConnection()));
    this->userName = userName;
    this->password = password;
    this->rootPath = rootPath;
    this->readOnly = readOnly;
    this->userslist = userslist;
    qDebug() << this->userslist;
    this->onlyOneIpAllowed = onlyOneIpAllowed;
}

bool FtpServer::isListening()
{
    return server->isListening();
}

bool FtpServer::isBlocking(bool tester2)
{
   // testing->isBlocking(true);
    testing->isBlocking=tester2;
}

void FtpServer::startNewControlConnection()
{
    QSslSocket *socket = (QSslSocket *) server->nextPendingConnection();

    // If this is not a previously encountered IP emit the newPeerIp signal.
    QString peerIp = socket->peerAddress().toString();
    qDebug() << "connection from" << peerIp;
    if (!encounteredIps.contains(peerIp)) {
        // If we don't allow more than one IP for the client, we close
        // that connection.
        if (onlyOneIpAllowed && !encounteredIps.isEmpty()) {
            delete socket;
            return;
        }

        emit newPeerIp(peerIp);
        encounteredIps.insert(peerIp);
    }

    // Create a new FTP control connection on this socket.
    testing = new FtpControlConnection(this, socket, rootPath, userName, password, readOnly,userslist);

}
#endif
