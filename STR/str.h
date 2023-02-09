#ifndef STR_H
#define STR_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QByteArray>
#include <QSettings>
#include <QFile>

#include <core.h>
#include <model.h>
#include <format.h>
#include "../include/common.h"

#include "include_modules.h"

#include "../network_client/network_client.h"
#include "../network_server/network_server.h"

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

private slots:

    void on_pushButtonSet_clicked();
    void on_pushButtonStart_clicked();
    void on_pushButtonStop_clicked();

    void on_pushButtonTest_clicked();

private:

    QTimer *STR_timer;

    M_TEST *model_test;
    QList<ModelQ *> modelList;
    QList<FormatQ *> formatList;

    Ui::STR *ui;

    QByteArray Buff;

    NetworkServer *STR_server;
    NetworkClient *STR_client;
    bool bServerNode;
    QString sHostName;
    int iHostPort;
    bool multiScreen;
};
#endif // STR_H
