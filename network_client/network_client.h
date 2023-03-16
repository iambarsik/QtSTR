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
    NetworkClient(STRNode NodeInfo, bool NA_mode = false);
    ~NetworkClient();

    bool isConnected() { return bConnected; }

    void addCommand(command_t command);

signals:
    void signalPackageFromServer(QByteArray);
    void signalCommandFromNA(command_na);
    void signalPackageFromNA(package_na);

private slots:
    void OnTimer();
    void discardSocket();
    void getErrorCode(QAbstractSocket::SocketError errorCode);

    void readyRead();

    void connectToServer();
    void slotStopReConnect();
    void slotStartReConnect();

    void sendMessage(command_type type);

private:
    QTcpSocket* socket;
    QByteArray bufferRead, bufferSend, dataSend;

    bool bConnected;
    bool bNA_MODE;
    QTimer *m_timer;
    QTimer *reconnect_timer;

    STRNode Node;
};

#endif // NETWORKCLIENT_H
