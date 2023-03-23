#include "model.h"

ModelQ::ModelQ(CoreQ *core, int id, QString name, QString object, QString system)
{
    this->core = core;
    this->id = id;
    this->model_name = name;
    this->model_object = object;
    this->model_system = system;
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

void ModelQ::sendCommandToModel(uint command, STR_PARAM par1, STR_PARAM par2, uint time)
{
    command_t com;
    com.code = command;
    com.par1 = par1;
    com.par2 = par2;
    com.time = time;
    emit signalCommandSended(com);
}

void ModelQ::sendCommandToNode(qint32 nodeId, uint command, qint32 par1, qint32 par2, uint time)
{
    command_na com;
    com.code = command;
    com.par1 = par1;
    com.par2 = par2;
    com.time = time;
    emit signalCommandToNodeSended(nodeId, com);
}
