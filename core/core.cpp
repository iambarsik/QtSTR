#include "core.h"

CoreQ::CoreQ():
    str_system_start(false)
{
    core_test_var1 = 0;
    core_test_var2 = 1;
    core_test_var3 = 2;
    core_test_var4 = 3;
    core_test_var5 = 4;
    core_test_var6 = 5;
    core_test_var7 = 6;
    core_test_double1 = 0.0;
    core_test_double2 = 0.0;
    core_test_double3 = 0.0;
    for(int i = 0; i < 1000; i++)  { core_test_arr[i] = 0; }
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

    // {
    // begin pack system

    pushInt((int)str_system_start);

    // end pack system
    // }

    pushInt(core_test_var1);
    pushInt(core_test_var2);
    pushInt(core_test_var3);
    pushInt(core_test_var4);
    pushInt(core_test_var5);
    pushInt(core_test_var6);
    pushInt(core_test_var7);

    for(int i = 0; i < 1000; i++)  {
        pushInt(core_test_arr[i]);
    }

    m_package.append(pushDouble(core_test_double1));
    m_package.append(pushDouble(core_test_double2));
    m_package.append(pushDouble(core_test_double3));

    return m_package;
}

void CoreQ::setCoreFromPackage(QByteArray package)
{
    QByteArray arr = package;

    // {
    // begin unpack system

    qint64 buff = 0;
    popInt(arr,buff); str_system_start = (bool) buff;

    // end unpack system
    // }

    popInt(arr,core_test_var1);
    popInt(arr,core_test_var2);
    popInt(arr,core_test_var3);
    popInt(arr,core_test_var4);
    popInt(arr,core_test_var5);
    popInt(arr,core_test_var6);
    popInt(arr,core_test_var7);

    for(int i = 0; i < 1000; i++)  {
        popInt(arr,core_test_arr[i]);
    }

    popDouble(arr,core_test_double1);
    popDouble(arr,core_test_double2);
    popDouble(arr,core_test_double3);
}

