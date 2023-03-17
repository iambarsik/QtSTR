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

#include <core.h>
#include <manager.h>
#include <model.h>
#include <format.h>
#include <network_client.h>
#include <network_server.h>
#include "../include/common.h"

    // include headers of system widgets
#include "formatcontainer.h"
#include "networkstate.h"

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

private:

signals:
    void init();
    void set();
    void start();
    void stop();
    void networkStateChanged();

private slots:
    void slotTimer();
    void slotReadCommand(command_t command);

public slots:
    void setClientInformation(int value);
    void slotOpenFormat(STRformat_enum name);
    void updateNetworkState();

private slots:
    void on_pushButtonSet_clicked();
    void on_pushButtonStart_clicked();
    void on_pushButtonStop_clicked();
    void on_action_triggered();
    void on_pushButtonContainer_clicked();

    void on_action_container_triggered();

    void on_action_networkstate_triggered();

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
