#include "str.h"
#include <QTextCodec>

void STR::ConnectModel(ModelQ *model)
{
    connect(this,&STR::set,model,&ModelQ::set);
    connect(this,&STR::start,model,&ModelQ::start);
    connect(this,&STR::stop,model,&ModelQ::stop);
}

void STR::ConnectFormat(FormatQ *format)
{
    connect(format,SIGNAL(signalCommandSended(command_t)),
            this,SLOT(slotReadCommand(command_t)));
}

void STR::slotReadCommand(command_t command)
{
    qDebug() << "Recieved command from format";
    qDebug() << "Command: " << command.code << ", pars: " << command.par1.toInt() << " " << command.par2.toInt();

    if(bServerNode) {

        switch(command.code)    {
            case 0xC0000003:            // init node ???

            break;
            case 0x80000001:            // set
                emit stop();
                emit set();
            break;
            case 0x80000002:            // start
                emit start();
            break;
            case 0x80000003:            // stop
                emit stop();
            break;
            case 0x80000008:            // end ???

            break;
            default: {

                uint model_id = core->commands.getModelOwner(command.code);
                if(model_id != 0) {
                    for(int i = 0; i < object_manager->modelList.size(); i++) {
                        if(model_id == object_manager->modelList[i]->getID())    {
                            object_manager->modelList[i]->ReadCommand(command);
                            break;
                        }
                    }
                    qDebug() << " " << core->commands.getCommandDescription(command.code);

                } else {
                    qDebug() << "STR :: Error in commands depending list";
                }

            } break;
        }

    } else {
        STR_client->addCommand(command);
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
