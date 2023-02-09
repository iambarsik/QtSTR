#include "commandsq.h"

CommandsQ::CommandsQ()
{
    dependings.push_back({ test_command1, 1000, "Тестовая команда 1"});
    dependings.push_back({ test_command2, 1000, "Тестовая команда 2"});
    dependings.push_back({ test_command3, 1000, "Тестовая команда 3"});
    dependings.push_back({ test_command4, 1000, "Тестовая команда 4"});
    dependings.push_back({ test_command5, 1000, "Тестовая команда 5"});
    dependings.push_back({ test_command6, 1000, "Тестовая команда 6"});
    dependings.push_back({ test_command7, 1000, "Тестовая команда 7"});
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
