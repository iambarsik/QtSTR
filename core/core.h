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

public:

    qint64 get_test_var1() { return core_test_var1; }
    qint64 get_test_var2() { return core_test_var2; }
    qint64 get_test_var3() { return core_test_var3; }
    qint64 get_test_var4() { return core_test_var4; }
    qint64 get_test_var5() { return core_test_var5; }
    qint64 get_test_var6() { return core_test_var6; }
    qint64 get_test_var7() { return core_test_var7; }

    qint64 get_test_arr(int position) { if(position < 1000) return core_test_arr[position]; else return -666; }

    double get_test_double1() { return core_test_double1; }
    double get_test_double2() { return core_test_double2; }
    double get_test_double3() { return core_test_double3; }

private:


    bool str_system_start;
    QByteArray m_package;

    qint64 core_test_var1; void set_test_var1(qint64 test_var1) { core_test_var1 = test_var1; }
    qint64 core_test_var2; void set_test_var2(qint64 test_var2) { core_test_var2 = test_var2; }
    qint64 core_test_var3; void set_test_var3(qint64 test_var3) { core_test_var3 = test_var3; }
    qint64 core_test_var4; void set_test_var4(qint64 test_var4) { core_test_var4 = test_var4; }
    qint64 core_test_var5; void set_test_var5(qint64 test_var5) { core_test_var5 = test_var5; }
    qint64 core_test_var6; void set_test_var6(qint64 test_var6) { core_test_var6 = test_var6; }
    qint64 core_test_var7; void set_test_var7(qint64 test_var7) { core_test_var7 = test_var7; }

    qint64 core_test_arr[1000]; void set_test_arr(qint64 value, int position) { if(position < 1000) core_test_arr[position] = value; }

    double core_test_double1; void set_test_double1(double test_double1) { core_test_double1 = test_double1; }
    double core_test_double2; void set_test_double2(double test_double2) { core_test_double2 = test_double2; }
    double core_test_double3; void set_test_double3(double test_double3) { core_test_double3 = test_double3; }



};

#endif // CORE_H
