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

class Q_DECL_EXPORT NetworkServer : public QObject
{
    Q_OBJECT
public:
    NetworkServer(qint32 Port);
    ~NetworkServer();


    void addCommand(command_t command);
    void addPackage(QByteArray package);

signals:
    void signalClientsAreConnected(int);
    void signalCommandFromClient(command_t);


private slots:

    void OnTimer();

    void newConnection();
    void appendToSocketList(QTcpSocket* socket);
    void discardSocket();
    void getErrorCode(QAbstractSocket::SocketError errorCode);

    void readyRead();

    void sendMessage(QTcpSocket* socket, command_type type);


private:
    QTcpServer* m_server;
    QSet<QTcpSocket*> connection_set;
    QByteArray bufferRead, bufferSend, dataSend;

    QTimer *m_timer;

    bool bInit;



};

#endif // NETWORK_H
