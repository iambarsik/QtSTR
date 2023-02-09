#ifndef MODEL_H
#define MODEL_H

#include "../../core/core.h"
#include "../../include/common.h"

#include <QTimer>

class Q_DECL_EXPORT ModelQ: public QObject
{
    Q_OBJECT
public:
    ModelQ(CoreQ *core, int id);
    CoreQ *core;

    void virtual init() {}
    void virtual set() {}
    void virtual update() {}
    void virtual inputFromCore() {}
    void virtual outputToCore() {}
    void virtual ReadCommand(command_t command) { Q_UNUSED(command); }

    uint getID() { return id; }

    void sendCommandToModel(uint command, int par1, int par2);
	
public slots:
    void start();
    void stop();
    void run();

signals:
     void signalCommandSended(command_t); 
	 
private:
    QTimer *model_timer;
    uint id = -1;


};

#endif // MODEL_H
