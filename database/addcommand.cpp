#include "addcommand.h"
#include "ui_addcommand.h"

#include <QMessageBox>

addcommand::addcommand(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addcommand)
{
    ui->setupUi(this);
}

addcommand::~addcommand()
{
    delete ui;
}

void addcommand::setData(const QStringList Models, const QStringList Types, bool EditMode, int id)
{
    ui->cbModel->clear();
    ui->cbPar1Type->clear();
    ui->cbPar2Type->clear();
    ui->cbModel->addItems(Models);
    ui->cbPar1Type->addItems(Types);
    ui->cbPar2Type->addItems(Types);
    this->EditMode = EditMode;
    this->ID = id;
    if(EditMode)
        setWindowTitle("Редактирование команды");
    else
        setWindowTitle("Добавление команды");
}

void addcommand::setCurrentData(command_struct com)
{
    ui->eName->setText(com.name);
    ui->cbPar1Type->setCurrentText(com.par1_type);
    ui->cbPar2Type->setCurrentText(com.par2_type);
    ui->cbModel->setCurrentText(com.model);
    ui->ePar1Com->setText(com.par1_text);
    ui->ePar2Com->setText(com.par2_text);
    ui->eDescription->setText(com.description);
}

void addcommand::on_bCancel_clicked()
{
    this->close();
}

void addcommand::on_bAdd_clicked()
{
    if(ui->eName->text().isEmpty() ||
       ui->eDescription->text().isEmpty() ||
       ui->cbModel->currentText().isEmpty())
    {
        QMessageBox::warning(this, "Ошибка!", "Заполните все поля!", QMessageBox::Yes);
        return;
    }
    command_struct com;
    com.name = ui->eName->text();
    com.par1_type = ui->cbPar1Type->currentText();
    com.par2_type = ui->cbPar2Type->currentText();
    com.par1_text = ui->ePar1Com->text();
    com.par2_text = ui->ePar2Com->text();
    com.model = ui->cbModel->currentText();
    com.description = ui->eDescription->text();
    if(EditMode)    {
        com.id = ID;
        emit signalUpdateCommand(com);
    } else {
        com.id = -1;
        emit signalAddCommand(com);
    }
    this->close();
}
