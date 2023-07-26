#ifndef COMMANDSQ_H
#define COMMANDSQ_H

#include <QObject>
#include <QList>

#include "../include/common.h"
#include "../include/network_def.h"

class Q_DECL_EXPORT CommandsQ : public QObject
{
    Q_OBJECT
public:
    CommandsQ();
    QString getCommandDescription(uint command_code);
    uint getModelOwner(uint command_code);
	int getEventCount() { return events.size(); }
    EventCommand getEvent(int num);

    void addEvent(uint command, qint32 time);
	
private:
    QList <STRCommand> dependings;
    QList <EventCommand> events;

public:

//<COMMANDS_SECTION>	
//</COMMANDS_SECTION>
};

#endif // COMMANDSQ_H
