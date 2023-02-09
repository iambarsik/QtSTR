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

void FormatQ::sendCommandToModel(uint command, int par1, int par2)
{
    emit signalCommandSended({command, par1, par2,0});
}

