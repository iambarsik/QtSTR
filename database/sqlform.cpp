#include "sqlform.h"
#include "ui_sqlform.h"

sqlform::sqlform(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sqlform)
{
    ui->setupUi(this);
}

sqlform::~sqlform()
{
    delete ui;
}

void sqlform::on_bCancel_clicked()
{
    this->close();
}

void sqlform::on_bExecute_clicked()
{
    QString sExecute = ui->eSQL->text();
    emit signalSQLrequest(sExecute);
    this->close();
}
