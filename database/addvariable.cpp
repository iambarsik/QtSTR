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

void addvariable::setCurrentData(QString name, QString model, QString type, uint size, QString description)
{
    ui->eName->setText(name);
    ui->cbModel->setCurrentText(model);
    ui->cbType->setCurrentText(type);
    ui->eSize->setText(QString("%1").arg(size));
    ui->eDescription->setText(description);
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
    if(EditMode)    {
        emit signalUpdateVariable(ID,
                                  ui->eName->text(),
                                  ui->cbModel->currentText(),
                                  ui->cbType->currentText(),
                                  ui->eSize->text().toUInt(),
                                  ui->eDescription->text());
    } else {
        emit signalAddVariable(ui->eName->text(),
                               ui->cbModel->currentText(),
                               ui->cbType->currentText(),
                               ui->eSize->text().toUInt(),
                               ui->eDescription->text());
    }
    this->close();
}

void addvariable::on_bCancel_clicked()
{
    this->close();
}
