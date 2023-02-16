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
