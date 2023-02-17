#include "format.h"

FormatQ::FormatQ(CoreQ *core, QWidget *parent)
    : QWidget(parent)
{
    this->core = core;

    format_timer = new QTimer;
    format_timer->setInterval(100);
    connect(format_timer, &QTimer::timeout,this, &FormatQ::update);
    format_timer->start();
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

