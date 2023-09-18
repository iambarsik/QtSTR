#include "str.h"
#include <QTextCodec>

void STR::ConnectModel(ModelQ *model)
{
    connect(this,&STR::set,model,&ModelQ::set);
    connect(this,&STR::start,model,&ModelQ::start);
    connect(this,&STR::stop,model,&ModelQ::stop);
    connect(model,SIGNAL(signalCommandToNodeSended(uint,command_na)),
            this,SLOT(sendCommandToNode(uint,command_na)));
    connect(model,SIGNAL(signalCommandSended(command_t)),
            this,SLOT(slotReadCommand(command_t)));
}

void STR::ConnectFormat(FormatQ *format)
{
    connect(format,SIGNAL(signalCommandSended(command_t)),
            this,SLOT(slotReadCommand(command_t)));
}

void STR::pushEvent(uint code, qint32 time)
{
    core->commands.addEvent(code, time);
    if(bServerNode == true && STR_server->getClientCount() > 0)    {
        command_t com;
        com.code = STR_COMMAND_ADD_EVENT;
        com.par1 = STR_PARAM(code);
        com.par2 = STR_PARAM(time);
        com.time = timeNow();
        STR_server->addCommand(com);
    }
    emit eventListChanged();
    qDebug () << "event is pushed " << code << time;
}

qint32 STR::timeNow()
{
    QTime t = QTime::currentTime();
    return t.msecsSinceStartOfDay();
}

void STR::slotReadCommand(command_t command)
{
    qDebug() << "Recieved command from format";
    qDebug() << "Command: " << command.code << ", pars: " << command.par1.toInt() << " " << command.par2.toInt();

    if(bServerNode) {

        switch(command.code)    {
            case STR_COMMAND_INIT_NODE:    //what to do????

            break;
            case STR_COMMAND_SET:
                emit stop();
                emit set();
            break;
            case STR_COMMAND_START:
                emit start();
            break;
            case STR_COMMAND_STOP:
                emit stop();
            break;
            case STR_COMMAND_END:         //what to do????

            break;
            default: {

                qint32 model_id = core->commands.getModelOwner(command.code);
                if(model_id != 0) {
                    for(int i = 0; i < object_manager->modelList.size(); i++) {
                        if(model_id == object_manager->modelList[i]->getID())    {
                            object_manager->modelList[i]->ReadCommand(command);

                            //pushEvent(command.code, command.time);
                            pushEvent(command.code, timeNow());
                            break;
                        }
                    }

                    //qDebug() << " " << core->commands.getCommandDescription(command.code);

                } else {
                    qDebug() << "STR :: Error in commands depending list";
                }

            } break;
        }

    } else {

        switch(command.code)    {
            case STR_COMMAND_INIT_NODE:            //what to do????

            break;
            case STR_COMMAND_ADD_EVENT:
                    // adding to event monitor                 d
                pushEvent(command.par1.toInt(), command.par2.toInt());
            break;
            default:
                    // send command to server
                STR_client->addCommand(command);
            break;
        }
    }
}

void STR::slotNewClientConnected(QTcpSocket *sock)
{
    for(int i = 0; i < core->commands.getEventCount(); i++) {
        EventCommand e = core->commands.getEvent(i);
        command_t command;
        command.code = STR_COMMAND_ADD_EVENT;
        command.par1 = e.code;
        command.par2 = e.time;
        command.time = timeNow();
        STR_server->addCommandToClient(sock,command);
    }
}

void STR::updateNetworkState()
{
    if(NodeInfo.isEmpty())  {
        for(int i = 0; i < NA_client.size(); i++)   {
            STRNodeInfo n;
            n.name = NA_client[i]->getNodeName();
            n.ID = NA_client[i]->getID();
            n.isNA = true;
            n.state = NA_client[i]->isConnected();
            NodeInfo.push_back(n);
        }
    } else {
        for(int i = 0; i < NA_client.size(); i++)   {
            for(int j = 0; j < NodeInfo.size(); j++)    {
                if(NA_client[i]->getID() == NodeInfo[j].ID) {

                        // if it was connected before, set the lost connection state
                    if(NA_client[i]->isConnected() == false && NodeInfo[j].state > 0)  {
                        NodeInfo[j].state = 2;
                    } else {
                        NodeInfo[j].state = NA_client[i]->isConnected();
                    }
                    break;
                }
            }
        }
    }

    emit networkStateChanged();

    for(int i = 0; i < NodeInfo.size(); i++)    {
        qDebug() << NodeInfo[i].ID;
    }
}

void STR::sendCommandToNode(uint nodeId, command_na com)
{
    if(bServerNode) {
        for(int i = 0; i < NA_client.size(); i++)   {
            if(NA_client[i]->getID() == (qint32)nodeId) {
                NA_client[i]->addCommandForNA(com.code, com.par1, com.par2);
                break;
            }
        }
    } else {

    }
}
