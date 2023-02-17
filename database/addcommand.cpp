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

void addcommand::setCurrentData(QString name, QString model, QString description)
{
    ui->eName->setText(name);
    ui->cbModel->setCurrentText(model);
    ui->eDescription->setText(description);
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
    if(EditMode)    {
        emit signalUpdateCommand(ID,
                                 ui->eName->text(),
                                 ui->cbPar1Type->currentText(),
                                 ui->cbPar2Type->currentText(),
                                 ui->ePar1Com->text(),
                                 ui->ePar2Com->text(),
                                 ui->cbModel->currentText(),
                                 ui->eDescription->text());
    } else {
        emit signalAddCommand(ui->eName->text(),
                              ui->cbPar1Type->currentText(),
                              ui->cbPar2Type->currentText(),
                              ui->ePar1Com->text(),
                              ui->ePar2Com->text(),
                              ui->cbModel->currentText(),
                              ui->eDescription->text());
    }
    this->close();
}
