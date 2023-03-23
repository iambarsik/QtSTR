#include "commandsq.h"

CommandsQ::CommandsQ()
{
    dependings.push_back({ STR_COMMAND_SET, 0, "<:: Отработка НУ ::>"});
    dependings.push_back({ STR_COMMAND_START, 0, "<:: Пуск динамики ::>"});
    dependings.push_back({ STR_COMMAND_STOP, 0, "<:: Стоп динамики ::>"});
    dependings.push_back({ STR_COMMAND_END, 0, "<:: Окончание тренировки ::>"});
//<CONSTRUCTOR_SECTION>	
//</CONSTRUCTOR_SECTION>
}

QString CommandsQ::getCommandDescription(uint command_code) {
    foreach(auto d, dependings)   {
        if(d.code == command_code)  {
            return d.description;
        }
    }
    return "unknown command";
}

uint CommandsQ::getModelOwner(uint command_code)    {
    foreach(auto d, dependings)   {
        if(d.code == command_code)  {
            return d.model_owner;
        }
    }
    return 0;
}

void CommandsQ::addEvent(uint command, qint32 time)  {
    events.push_back({command, time});
}