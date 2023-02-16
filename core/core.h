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

    friend class M_TEST;

public:
    CoreQ();

    CommandsQ commands;
    // {
    // begin methods for puck/unpack core data for network

    void pushInt(qint32 value);
    void popInt(QByteArray &arr, int &variable);
    QByteArray pushDouble(double value);
    void popDouble(QByteArray &arr, double &variable);
    void pushChar(char value);
    void popChar(QByteArray &arr, char &variable);

    // }
    // end methods for puck/unpack core data for network

    // {
    // begin of system methods

    bool isStartCore() { return str_system_start; }
    QByteArray getPackage();

    // end of system methods
    // }


public slots:
    void setCoreFromPackage(QByteArray arr);
    void startCore() { str_system_start = true; }
    void stopCore() { str_system_start = false; }

public:
    // {
    // begin of getters for variables

    qint32 get_test_var1() { return core_test_var1; }
    qint32 get_test_var2() { return core_test_var2; }
    qint32 get_test_var3() { return core_test_var3; }
    qint32 get_test_var4() { return core_test_var4; }
    qint32 get_test_var5() { return core_test_var5; }
    qint32 get_test_var6() { return core_test_var6; }
    qint32 get_test_var7() { return core_test_var7; }

    qint32 get_test_arr(int position) { if(position < 1000) return core_test_arr[position]; else return -666; }

    double get_test_double1() { return core_test_double1; }
    double get_test_double2() { return core_test_double2; }
    double get_test_double3() { return core_test_double3; }

    // end of getters for variables
    // }


private:

    // {
    // begin of system variables

    bool str_system_start;
    QByteArray m_package;

    // end of system variables
    // }

    // {
    // begin database generated variables

    qint32 core_test_var1; void set_test_var1(int test_var1) { core_test_var1 = test_var1; }
    qint32 core_test_var2; void set_test_var2(int test_var2) { core_test_var2 = test_var2; }
    qint32 core_test_var3; void set_test_var3(int test_var3) { core_test_var3 = test_var3; }
    qint32 core_test_var4; void set_test_var4(int test_var4) { core_test_var4 = test_var4; }
    qint32 core_test_var5; void set_test_var5(int test_var5) { core_test_var5 = test_var5; }
    qint32 core_test_var6; void set_test_var6(int test_var6) { core_test_var6 = test_var6; }
    qint32 core_test_var7; void set_test_var7(int test_var7) { core_test_var7 = test_var7; }

    qint32 core_test_arr[1000]; void set_test_arr(int value, int position) { if(position < 1000) core_test_arr[position] = value; }

    double core_test_double1; void set_test_double1(double test_double1) { core_test_double1 = test_double1; }
    double core_test_double2; void set_test_double2(double test_double2) { core_test_double2 = test_double2; }
    double core_test_double3; void set_test_double3(double test_double3) { core_test_double3 = test_double3; }

    // end database generated variables
    // }


};

#endif // CORE_H
