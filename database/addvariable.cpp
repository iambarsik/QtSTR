#include "addvariable.h"
#include "ui_addvariable.h"

#include <QMessageBox>

addvariable::addvariable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addvariable)
{
    ui->setupUi(this);
}

addvariable::~addvariable()
{
    delete ui;
}

void addvariable::setData(const QStringList Models, const QStringList Types, bool EditMode, int id)
{
    ui->cbModel->clear();
    ui->cbType->clear();
    ui->cbModel->addItems(Models);
    ui->cbType->addItems(Types);
    this->EditMode = EditMode;
    this->ID = id;
    if(EditMode)
        setWindowTitle("Редактирование переменной");
    else
        setWindowTitle("Добавление переменной");
}

void addvariable::setCurrentData(variable_struct var)
{
    ui->eName->setText(var.name);
    ui->cbModel->setCurrentText(var.model);
    ui->cbType->setCurrentText(var.type);
    ui->eSize->setText(QString("%1").arg(var.size));
    ui->eDescription->setText(var.description);
}

void addvariable::on_bAdd_clicked()
{
    if(ui->eName->text().isEmpty() ||
       ui->eDescription->text().isEmpty() ||
       ui->eSize->text().isEmpty() ||
       ui->eSize->text().toInt() < 0 ||
       ui->cbType->currentText().isEmpty())
    {
        QMessageBox::warning(this, "Ошибка!", "Заполните все поля!", QMessageBox::Yes);
        return;
    }
    variable_struct var;
    var.name = ui->eName->text();
    var.model = ui->cbModel->currentText();
    var.type = ui->cbType->currentText();
    var.size = ui->eSize->text().toUInt();
    var.description = ui->eDescription->text();
    if(EditMode)    {
        var.id = ID;
        emit signalUpdateVariable(var);
    } else {
        var.id = -1;
        emit signalAddVariable(var);
    }
    this->close();
}

void addvariable::on_bCancel_clicked()
{
    this->close();
}
