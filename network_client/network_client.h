#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>
#include <QException>

#include "../include/common.h"
#include "../include/network_def.h"

class Q_DECL_EXPORT NetworkClient : public QObject
{
    Q_OBJECT
public:
    NetworkClient(STRNode NodeInfo, bool NA_mode = false);
    ~NetworkClient();

    bool isConnected() { return bConnected; }

    QString getNodeName() { return Node.name; }
    qint32 getID() { return Node.ID; }

    void addCommand(command_t command);
    void addCommandForNA(qint32 command, qint32 par1, qint32 par2); // not works yet

signals:
    void signalPackageFromServer(QByteArray);
    void signalCommandFromServer(command_t);
    void signalCommandFromNA(command_na);
    void signalPackageFromNA(package_na);
    void clientConnected();
    void clientDisconnected();

private slots:
    void OnTimer();
    void discardSocket();
    void getErrorCode(QAbstractSocket::SocketError errorCode);

    void readyRead();

    void connectToServer();
    void slotStopReConnect();
    void slotStartReConnect();

    void sendMessage(network_command_type type);

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
