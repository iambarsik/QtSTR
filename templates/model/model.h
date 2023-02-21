#ifndef MODEL_H
#define MODEL_H

#include "../../core/core.h"
#include "../../include/common.h"

#include <QTimer>

class Q_DECL_EXPORT ModelQ: public QObject
{
    Q_OBJECT
public:
    ModelQ(CoreQ *core, int id, QString name, QString object, QString system);
    CoreQ *core;

    void virtual init() {}
    void virtual set() {}
    void virtual update() {}
    void virtual inputFromCore() {}
    void virtual outputToCore() {}
    void virtual ReadCommand(command_t command) { Q_UNUSED(command); }

    uint getID() { return id; }

    void sendCommandToModel(uint command, STR_PARAM par1, STR_PARAM par2, qint32 time);
	
public slots:
    void start();
    void stop();
    void run();

signals:
     void signalCommandSended(command_t); 
	 
private:
    QTimer *model_timer;
    uint id = -1;
    QString model_name = "unnamed";
    QString model_object = "noobject";
    QString model_system = "nosystem";


};

#endif // MODEL_H
