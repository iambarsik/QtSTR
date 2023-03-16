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
    QString getModelName() { return model_name; }
    QString getModelObject() { return model_object; }
    QString getModelSystem() { return model_system; }

    void sendCommandToModel(uint command, STR_PARAM par1, STR_PARAM par2, qint32 time);
	
public slots:
    void start();
    void stop();
    void run();
    virtual void ReadCommandFromNA(command_na command) { Q_UNUSED(command); };
    virtual void ReadPackageFromNA(package_na package) { Q_UNUSED(package); };

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
