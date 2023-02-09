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

    // all command codes generated from database

    const uint test_command1 = 1001;
    const uint test_command2 = 1002;
    const uint test_command3 = 1003;
    const uint test_command4 = 1004;
    const uint test_command5 = 1005;
    const uint test_command6 = 1006;
    const uint test_command7 = 1007;




};

#endif // COMMANDSQ_H
