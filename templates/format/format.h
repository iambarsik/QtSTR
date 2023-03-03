#ifndef FORMAT_H
#define FORMAT_H

#include "../../core/core.h"
#include "../../include/common.h"

#include <QTimer>
#include <QWidget>

class Q_DECL_EXPORT FormatQ : public QWidget
{
    Q_OBJECT
public:
    FormatQ(CoreQ *core, uint id, QString name, QString object, QString system, QWidget *parent = nullptr );
    ~FormatQ();

    CoreQ *core;

    uint getID() { return id; }

    void sendCommandToModel(uint command, STR_PARAM par1, STR_PARAM par2, qint32 time);


signals:
     void signalCommandSended(command_t);

public slots:
    virtual void init() {};
    virtual void update() {};

private:     
     QTimer *format_timer;
     uint id = -1;
     QString format_name = "unnamed";
     QString format_object = "noobject";
     QString format_system = "nosystem";
};

#endif // FORMAT_H
