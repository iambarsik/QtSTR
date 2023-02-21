#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>
#include <QException>

#include "../include/common.h"

class Q_DECL_EXPORT NetworkClient : public QObject
{
    Q_OBJECT
public:
    NetworkClient(QString Host, qint32 Port);
    ~NetworkClient();

    void addCommand(command_t command);

signals:
    void signalPackageFromServer(QByteArray);

private slots:

    void OnTimer();

    void discardSocket();
    void getErrorCode(QAbstractSocket::SocketError errorCode);

    void readyRead();

    void sendMessage(command_type type);

private:
    QTcpSocket* socket;
    QByteArray bufferRead, bufferSend, dataSend;

    bool bConnected;
    QTimer *m_timer;
    bool bInit;
};

#endif // NETWORKCLIENT_H
