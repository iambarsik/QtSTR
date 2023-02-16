#ifndef COMMANDSQ_H
#define COMMANDSQ_H

#include <QObject>
#include <QList>

#include "../include/common.h"

class Q_DECL_EXPORT CommandsQ : public QObject
{
    Q_OBJECT
public:
    CommandsQ();
    QString getCommandDescription(uint command_code);
    uint getModelOwner(uint command_code);

private:
    QList <STRCommand> dependings;

public:

//<COMMANDS_SECTION>	
//</COMMANDS_SECTION>
};

#endif // COMMANDSQ_H
