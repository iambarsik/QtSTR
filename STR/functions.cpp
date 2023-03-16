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
