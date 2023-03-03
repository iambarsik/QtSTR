#ifndef CORE_H
#define CORE_H

#include <QtCore/qglobal.h>
#include <QDataStream>
#include <QByteArray>
#include <QDebug>

#include "commandsq.h"


class Q_DECL_EXPORT CoreQ : public QObject
{
    Q_OBJECT

//<FRIEND_MODEL_SECTION>	
//</FRIEND_MODEL_SECTION>

public:
    CoreQ();

    CommandsQ commands;

    void pushInt(qint64 value);
    void popInt(QByteArray &arr, qint64 &variable);
    QByteArray pushDouble(double value);
    void popDouble(QByteArray &arr, double &variable);
    void pushChar(char value);
    void popChar(QByteArray &arr, char &variable);

    bool isStartCore() { return str_system_start; }
    QByteArray getPackage();

public slots:
    void setCoreFromPackage(QByteArray arr);
    void startCore() { str_system_start = true; }
    void stopCore() { str_system_start = false; }

private:
    bool str_system_start;
    QByteArray m_package;
	
public:
//<GET_SECTION>
//</GET_SECTION>

private:
//<SET_SECTION>	
//</SET_SECTION>

//<VAR_SECTION>	
//</VAR_SECTION>
};

#endif // CORE_H
