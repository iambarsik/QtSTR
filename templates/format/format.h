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
    FormatQ(CoreQ *core, QWidget *parent = nullptr );
    CoreQ *core;

    void sendCommandToModel(uint command, STR_PARAM par1, STR_PARAM par2, qint32 time);

    QTimer *format_timer;

signals:
     void signalCommandSended(command_t);

public slots:
    virtual void init() {};
    virtual void update() {};

private:

};

#endif // FORMAT_H
