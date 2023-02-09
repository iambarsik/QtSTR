#include "model.h"

ModelQ::ModelQ(CoreQ *core, int id)
{
    this->core = core;
    this->id = id;
    model_timer = new QTimer;
    model_timer->setInterval(100);
    connect(model_timer, &QTimer::timeout,this, &ModelQ::run);
}

void ModelQ::start()
{
    model_timer->start();
}

void ModelQ::stop()
{
    model_timer->stop();
}

void ModelQ::run()
{
    inputFromCore();
    update();
    outputToCore();
}

void ModelQ::sendCommandToModel(uint command, int par1, int par2)
{
    emit signalCommandSended({command, par1, par2,0});
}
