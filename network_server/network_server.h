#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QDebug>
#include <QSet>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>

#include "../include/common.h"
#include "../include/network_def.h"

class Q_DECL_EXPORT NetworkServer : public QObject
{
    Q_OBJECT
public:
    NetworkServer(STRNode NodeInfo);
    ~NetworkServer();

    void addCommand(command_t command);
    void addCommandToClient(QTcpSocket* sock, command_t command);
    void addPackage(QByteArray package);

    qint32 getClientCount() { return connection_set.size(); }

signals:
    void signalNewClientConnected(QTcpSocket *sock);
    void signalClientsAreConnected(int);
    void signalCommandFromClient(command_t);

private slots:

    void OnTimer();

    void newConnection();
    void appendToSocketList(QTcpSocket* socket);
    void discardSocket();
    void getErrorCode(QAbstractSocket::SocketError errorCode);

    void readyRead();

    void sendMessage(QTcpSocket* socket, network_command_type type);

private:
    QTcpServer* m_server;
    QSet<QTcpSocket*> connection_set;
    QByteArray bufferRead, bufferSend, dataSend;

    QTimer *m_timer;

    STRNode Node;
};

#endif // NETWORK_H
