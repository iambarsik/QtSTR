#ifndef STR_H
#define STR_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QDebug>
#include <QTimer>
#include <QByteArray>
#include <QSettings>
#include <QFile>
#include <QVariant>
#include <QMessageBox>
#include <QTime>

#include <core.h>
#include <manager.h>
#include <model.h>
#include <format.h>
#include <network_client.h>
#include <network_server.h>

#include "../include/common.h"
#include "../include/network_def.h"

    // include headers of system widgets
#include "formatcontainer.h"
#include "networkstate.h"
#include "eventmonitor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class STR; }
QT_END_NAMESPACE


class STR : public QMainWindow
{
    Q_OBJECT

public:
    STR(QWidget *parent = nullptr);
    ~STR();

    CoreQ *core;
    ManagerQ *object_manager;

    void ConnectModel(ModelQ *model);
    void ConnectFormat(FormatQ *format);
    bool isMultiScreen() { return multiScreen; }
    void pushEvent(uint code, qint32 time);

    qint32 timeNow();

private:

signals:
    void init();
    void set();
    void start();
    void stop();
    void networkStateChanged();
    void eventListChanged();

private slots:
    void slotTimer();
    void slotReadCommand(command_t command);


public slots:
    void slotNewClientConnected(QTcpSocket *sock);
    void setClientInformation(int value);
    void slotOpenFormat(STRformat_enum name);
    void updateNetworkState();

    void sendCommandToNode(uint nodeId, command_na com);

private slots:
    void on_pushButtonSet_clicked();
    void on_pushButtonStart_clicked();
    void on_pushButtonStop_clicked();
    void on_action_triggered();
    void on_pushButtonContainer_clicked();

    void on_action_container_triggered();

    void on_action_networkstate_triggered();

    void on_pushButtonContainer_2_clicked();

    void on_action_eventmonitor_triggered();

private:

    QTimer *STR_timer;

    Ui::STR *ui;

    QByteArray Buff;

    NetworkServer *STR_server;
    NetworkClient *STR_client;

    STRNode CurrentNode;
    QList<STRNode> NA_Nodes;
    QList<NetworkClient *> NA_client;
    QList<STRNodeInfo> NodeInfo;

    bool bServerNode;
    bool multiScreen;

};
#endif // STR_H
