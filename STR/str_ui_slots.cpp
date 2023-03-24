#include "str.h"
#include "ui_str.h"

void STR::on_pushButtonSet_clicked()
{
    if(bServerNode) {
        emit stop();
        emit set();
        pushEvent(STR_COMMAND_STOP, timeNow());
        pushEvent(STR_COMMAND_SET, timeNow());
    } else {
        command_t com;
        com.code = STR_COMMAND_SET;
        com.par1 = STR_PARAM((qint64)0);
        com.par2 = STR_PARAM((qint64)0);
        com.time = 0;
        STR_client->addCommand(com);
    }
}

void STR::on_pushButtonStart_clicked()
{
    if(bServerNode) {
        emit start();
        pushEvent(STR_COMMAND_START, timeNow());
    } else {
        command_t com;
        com.code = STR_COMMAND_START;
        com.par1 = STR_PARAM((qint64)0);
        com.par2 = STR_PARAM((qint64)0);
        com.time = 0;
        STR_client->addCommand(com);
    }
}

void STR::on_pushButtonStop_clicked()
{
    if(bServerNode) {
        emit stop();
        pushEvent(STR_COMMAND_STOP, timeNow());
    } else {
        command_t com;
        com.code = STR_COMMAND_STOP;
        com.par1 = STR_PARAM((qint64)0);
        com.par2 = STR_PARAM((qint64)0);
        com.time = 0;
        STR_client->addCommand(com);
    }
}

void STR::on_action_triggered()
{
    exit(EXIT_SUCCESS);
}

void STR::on_pushButtonContainer_clicked()
{
    formatcontainer *f = new formatcontainer(object_manager->getFormatList());
    connect(f, SIGNAL(signalOpenFormat(STRformat_enum)),this, SLOT(slotOpenFormat(STRformat_enum)));
    ui->mdiArea->addSubWindow(f);
    f->show();
}

void STR::on_action_container_triggered()
{
    formatcontainer *f = new formatcontainer(object_manager->getFormatList());
    connect(f, SIGNAL(signalOpenFormat(STRformat_enum)),this, SLOT(slotOpenFormat(STRformat_enum)));
    ui->mdiArea->addSubWindow(f);
    f->show();
}

void STR::on_action_networkstate_triggered()
{
    networkstate *f = new networkstate(&NodeInfo);
    connect(this, SIGNAL(networkStateChanged()),f, SLOT(update()));
    ui->mdiArea->addSubWindow(f);
    f->show();
}

void STR::on_action_eventmonitor_triggered()
{
    eventmonitor *f = new eventmonitor(core);
    connect(this, SIGNAL(eventListChanged()),f, SLOT(update()));
    ui->mdiArea->addSubWindow(f);
    f->show();
}

void STR::on_pushButtonContainer_2_clicked()
{
/*
    for(int i = 0; i < 100; i++)    {
        STR_server->addCommand({(uint)5000 + i,i,i,i});
    }
*/
/*
    for(int i = 0; i < core->commands.getEventCount(); i++) {
        EventCommand e = core->commands.getEvent(i);
        command_t command;
        command.code = STR_COMMAND_ADD_EVENT;
        command.par1 = e.code;
        command.par2 = e.time;
        command.time = timeNow();
        STR_server->addCommandToClient(SSS,command);
    }
*/
}
