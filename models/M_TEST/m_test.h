#ifndef M_TEST_H
#define M_TEST_H

#define MODEL_ID    1000

#include <QDebug>

#include "../../templates/model/model.h"
#include "../../core/core.h"
#include "../../include/common.h"

#define test_command1 1001 // Тестовая команда 1
#define test_command2 1002 // Тестовая команда 2
#define test_command3 1003 // Тестовая команда 3
#define test_command4 1004 // Тестовая команда 4
#define test_command5 1005 // Тестовая команда 5
#define test_command6 1006 // Тестовая команда 6
#define test_command7 1007 // Тестовая команда 7

class Q_DECL_EXPORT M_TEST: public ModelQ
{
public:
    M_TEST(CoreQ * core);

        // overrided methods
    void inputFromCore() override;
    void outputToCore() override;
    void init() override;
    void set() override;
    void update() override;
    void ReadCommand(command_t command) override;

private:
    qint32 test_var1;
    qint32 test_var2;
    qint32 test_var3;
    qint32 test_var4;
    qint32 test_var5;
    qint32 test_var6;
    qint32 test_var7;

    qint32 test_arr[1000];

    double test_double1;
    double test_double2;
    double test_double3;

};

#endif // M_TEST_H
