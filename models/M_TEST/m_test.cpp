#include "m_test.h"

M_TEST::M_TEST(CoreQ * core)
    : ModelQ(core, MODEL_ID, "M_TEST", "НЭМ", "СУБА")
{

}

void M_TEST::inputFromCore()
{
    test_var1 = core->get_test_var1();
    test_var2 = core->get_test_var2();
    test_var3 = core->get_test_var3();
    test_var4 = core->get_test_var4();
    test_var5 = core->get_test_var5();
    test_var6 = core->get_test_var6();
    test_var7 = core->get_test_var7();

    for(int i = 0; i < 1000; i++) test_arr[i] = core->get_test_arr(i);

    test_double1 = core->get_test_double1();
    test_double2 = core->get_test_double2();
    test_double3 = core->get_test_double3();
}

void M_TEST::outputToCore()
{
    core->set_test_var1(test_var1);
    core->set_test_var2(test_var2);
    core->set_test_var3(test_var3);
    core->set_test_var4(test_var4);
    core->set_test_var5(test_var5);
    core->set_test_var6(test_var6);
    core->set_test_var7(test_var7);

    for(int i = 0; i < 1000; i++) core->set_test_arr(test_arr[i],i);

    core->set_test_double1(test_double1);
    core->set_test_double2(test_double2);
    core->set_test_double3(test_double3);
}

void M_TEST::init()
{
    test_var1 = 0;
    test_var2 = 0;
    test_var3 = 0;
    test_var4 = 0;
    test_var5 = 0;
    test_var6 = 0;
    test_var7 = 0;
    for(int i = 0; i < 1000; i++) test_arr[i] = 0;
    test_double1 = 0.0;
    test_double2 = 0.0;
    test_double3 = 0.0;


    outputToCore();
}

void M_TEST::set()
{
    test_var1 = 0;
    test_var2 = 0;
    test_var3 = 0;
    test_var4 = 0;
    test_var5 = 0;
    test_var6 = 0;
    test_var7 = 0;
    for(int i = 0; i < 1000; i++) test_arr[i] = 0;
    test_double1 = 0.0;
    test_double2 = 0.0;
    test_double3 = 0.0;


    outputToCore();    
}

void M_TEST::update()
{
    test_var1++;
    if(test_var1 > 20) test_var1 = 0;
    test_var2++;
    if(test_var2 > 20) test_var2 = 0;
    test_var3++;
    test_var4+=2;
    test_var5+=3;
    test_var6+=4;
    test_var7+=5;

    test_arr[995] += 13;
    test_arr[996] += 14;
    test_arr[997] += 15;
    test_arr[998] += 16;
    test_arr[999] += 17;

    test_double1 += 0.12341;
    test_double2 += 0.0123;
    test_double3 += 0.01231;
}

void M_TEST::ReadCommand(command_t command)
{
    switch(command.code)    {
        case test_command1: {

        } break;
        case test_command2: {

        } break;
        case test_command3: {

        } break;
        case test_command4: {

        } break;
        case test_command5: {

        } break;
        case test_command6: {

        } break;
        case test_command7: {
            double p1 = command.par1.toDouble();
            double p2 = command.par2.toDouble();

            test_double3 = p1;
            Q_UNUSED(p1);
            Q_UNUSED(p2);
        } break;
        default:
            qDebug() << "Unknown command in model ID = " << getID();
        break;
    }

    qDebug() << "Got command " << command.code << "in model ID = : " << getID();

    outputToCore();
}
