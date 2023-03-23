#include "format.h"

FormatQ::FormatQ(CoreQ *core, uint id, QString name, QString object, QString system, QWidget *parent)
    : QWidget(parent)
{
    this->core = core;
    this->id = id;
    this->format_name = name;
    this->format_object = object;
    this->format_system = system;

    format_timer = new QTimer;
    format_timer->setInterval(100);
    connect(format_timer, &QTimer::timeout,this, &FormatQ::update);
    format_timer->start();
}

FormatQ::~FormatQ()
{
    format_timer->stop();
    this->disconnect();
    delete format_timer;
    core = nullptr;
}

void FormatQ::sendCommandToModel(uint command, STR_PARAM par1, STR_PARAM par2, qint32 time)
{
    command_t com;
    com.code = command;
    com.par1 = par1;
    com.par2 = par2;
    com.time = time;
    emit signalCommandSended(com);
}

void FormatQ::sendCommandToNode(qint32 nodeId, uint command, qint32 par1, qint32 par2, qint32 time)
{
    command_na com;
    com.code = command;
    com.par1 = par1;
    com.par2 = par2;
    com.time = time;
    emit signalCommandToNodeSended(nodeId, com);
}

