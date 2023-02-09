#include "f_test.h"
#include "ui_f_test.h"

F_TEST::F_TEST(CoreQ * core, QWidget *parent)
    : FormatQ(core, parent)
    , ui(new Ui::F_TEST)
{
    ui->setupUi(this);
}

F_TEST::~F_TEST()
{
    delete ui;
}

void F_TEST::update()
{
    ui->textEdit->clear();
    ui->textEdit->append(QString("var1 = %1").arg(core->get_test_var1()));
    ui->textEdit->append(QString("var2 = %1").arg(core->get_test_var2()));
    ui->textEdit->append(QString("var3 = %1").arg(core->get_test_var3()));
    ui->textEdit->append(QString("var4 = %1").arg(core->get_test_var4()));
    ui->textEdit->append(QString("var5 = %1").arg(core->get_test_var5()));
    ui->textEdit->append(QString("var6 = %1").arg(core->get_test_var6()));
    ui->textEdit->append(QString("var7 = %1").arg(core->get_test_var7()));
    ui->textEdit->append(QString("arr995 = %1").arg(core->get_test_arr(995)));
    ui->textEdit->append(QString("arr996 = %1").arg(core->get_test_arr(996)));
    ui->textEdit->append(QString("arr997 = %1").arg(core->get_test_arr(997)));
    ui->textEdit->append(QString("arr998 = %1").arg(core->get_test_arr(998)));
    ui->textEdit->append(QString("arr999 = %1").arg(core->get_test_arr(999)));
    ui->textEdit->append(QString("double1 = %1").arg(core->get_test_double1()));
    ui->textEdit->append(QString("double2 = %1").arg(core->get_test_double2()));
    ui->textEdit->append(QString("double3 = %1").arg(core->get_test_double3()));
}


void F_TEST::on_pushButton_clicked()
{
    ui->textEdit->append(QString("test_var1 = %1").arg(core->get_test_var1()));

}

void F_TEST::on_pushButton_2_clicked()
{
    sendCommandToModel(core->commands.test_command7, 4, 6);
    //emit signalCommandSended({ 0x00123, 4, 6, 0 });
}
