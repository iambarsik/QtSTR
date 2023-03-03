#include "core.h"

CoreQ::CoreQ()
{
//<CONSTRUCTOR_SECTION>	
//</CONSTRUCTOR_SECTION>
}

void CoreQ::pushInt(qint64 value)
{
    m_package.append((value >> 56) & 0xFF);
    m_package.append((value >> 48) & 0xFF);
    m_package.append((value >> 40) & 0xFF);
    m_package.append((value >> 32) & 0xFF);
    m_package.append((value >> 24) & 0xFF);
    m_package.append((value >> 16) & 0xFF);
    m_package.append((value >> 8)  & 0xFF);
    m_package.append((value >> 0)  & 0xFF);
}

void CoreQ::popInt(QByteArray &arr, qint64 &variable)
{
    bool ok;
    variable = arr.mid(0,sizeof(variable)).toHex().toInt(&ok,16); arr.remove(0,sizeof (variable));
}

QByteArray CoreQ::pushDouble(double value)
{
    QByteArray arr;
    QDataStream stream(&arr, QIODevice::WriteOnly);
    stream << value;
    return  arr;
}

void CoreQ::popDouble(QByteArray &arr, double &variable)
{
    QDataStream stream(arr);
    stream >> variable;
    arr.remove(0,sizeof (variable));
}

void CoreQ::pushChar(char value)
{
    m_package.append(value);
}

void CoreQ::popChar(QByteArray &arr, char &variable)
{
    QByteArray c = arr.mid(0,sizeof(variable)); arr.remove(0,sizeof (variable));
    const char *value = c.constData();
    variable = *value;
    delete value;
}

QByteArray CoreQ::getPackage()
{
    m_package.clear();
    pushInt((int)str_system_start);
	
//<PACK_SECTION>
//</PACK_SECTION>
	
    return m_package;
}

void CoreQ::setCoreFromPackage(QByteArray package)
{
    QByteArray arr = package;
    qint64 buff = 0;
    popInt(arr,buff); str_system_start = (bool) buff;

//<UNPACK_SECTION>
//</UNPACK_SECTION>
}

