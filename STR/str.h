#ifndef STR_H
#define STR_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QDebug>
#include <QTimer>
#include <QByteArray>
#include <QSettings>
#include <QFile>

#include <core.h>
#include <manager.h>
#include <model.h>
#include <format.h>
#include <network_client.h>
#include <network_server.h>
#include "../include/common.h"

#include "formatcontainer.h"

//#include "../network_client/network_client.h"
//#include "../network_server/network_server.h"

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
    void initModels();

signals:
    void init();
    void set();
    void start();
    void stop();

private slots:
    void slotTimer();
    void slotReadCommand(command_t command);

public slots:
    void setClientInformation(int value);
    void slotOpenFormat(STRformat_enum name);

private slots:

    void on_pushButtonSet_clicked();
    void on_pushButtonStart_clicked();
    void on_pushButtonStop_clicked();

    void on_action_triggered();

    void on_pushButtonContainer_clicked();

private:

    QTimer *STR_timer;

    Ui::STR *ui;

    QByteArray Buff;

    NetworkServer *STR_server;
    NetworkClient *STR_client;

    STRNode CurrentNode;

    bool bServerNode;
    bool multiScreen;
};
#endif // STR_H
