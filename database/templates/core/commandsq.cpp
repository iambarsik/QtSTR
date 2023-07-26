#include "commandsq.h"

CommandsQ::CommandsQ()
{
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

EventCommand CommandsQ::getEvent(int num)
{
    if(num > - 1 && num < events.size()) {
        return events[num];
    }
    const uint com = 0;
    return {com,0};
}

void CommandsQ::addEvent(uint command, qint32 time)  {
    events.push_back({command, time});
}