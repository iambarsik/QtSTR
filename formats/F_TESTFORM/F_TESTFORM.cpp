#include "F_TESTFORM.h"
#include "ui_F_TESTFORM.h"

F_TESTFORM::F_TESTFORM(CoreQ * core, QWidget *parent)
    : FormatQ(core, FORMAT_ID, "F_TESTFORM", "НЭМ", "СУБА", parent)
    , ui(new Ui::F_TESTFORM)
{
    ui->setupUi(this);
}

F_TESTFORM::~F_TESTFORM()
{
    delete ui;
}

void F_TESTFORM::update()
{
    ui->textEdit->clear();
    ui->textEdit->append(QString("another var1 = %1").arg(core->get_test_var1()));
    ui->textEdit->append(QString("another var2 = %1").arg(core->get_test_var2()));
    ui->textEdit->append(QString("another var3 = %1").arg(core->get_test_var3()));
    ui->textEdit->append(QString("another var4 = %1").arg(core->get_test_var4()));
    ui->textEdit->append(QString("another var5 = %1").arg(core->get_test_var5()));
    ui->textEdit->append(QString("another var6 = %1").arg(core->get_test_var6()));
    ui->textEdit->append(QString("another var7 = %1").arg(core->get_test_var7()));
    ui->textEdit->append(QString("another arr995 = %1").arg(core->get_test_arr(995)));
    ui->textEdit->append(QString("another arr996 = %1").arg(core->get_test_arr(996)));
    ui->textEdit->append(QString("another arr997 = %1").arg(core->get_test_arr(997)));
    ui->textEdit->append(QString("another arr998 = %1").arg(core->get_test_arr(998)));
    ui->textEdit->append(QString("another arr999 = %1").arg(core->get_test_arr(999)));
    ui->textEdit->append(QString("another double1 = %1").arg(core->get_test_double1()));
    ui->textEdit->append(QString("another double2 = %1").arg(core->get_test_double2()));
    ui->textEdit->append(QString("another double3 = %1").arg(core->get_test_double3()));
}


void F_TESTFORM::on_pushButton_clicked()
{
    ui->textEdit->append(QString("test_var1 = %1").arg(core->get_test_var1()));
}

void F_TESTFORM::on_pushButton_2_clicked()
{
    sendCommandToModel(core->commands.test_command7,
                       STR_PARAM(ui->ep1->text().toDouble()),
                       STR_PARAM(ui->ep2->text().toDouble()), 0);
}
