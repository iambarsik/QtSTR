#include "addmodel.h"
#include "ui_addmodel.h"

#include <QMessageBox>

addmodel::addmodel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addform)
{
    ui->setupUi(this);
}

addmodel::~addmodel()
{
    delete ui;
}

void addmodel::setData(const QStringList Objects, const QStringList Systems, bool EditMode, int id)
{
    ui->cbObject->clear();
    ui->cbSystem->clear();
    ui->cbObject->addItems(Objects);
    ui->cbSystem->addItems(Systems);
    this->EditMode = EditMode;
    this->ID = id;
}

void addmodel::on_bCancel_clicked()
{
    this->close();
}

void addmodel::on_bAdd_clicked()
{
    if(ui->eName->text().isEmpty() ||
       ui->eDescription->text().isEmpty() ||
       ui->cbObject->currentText().isEmpty() ||
       ui->cbSystem->currentText().isEmpty())
    {
        QMessageBox::warning(this, "Ошибка!", "Заполните все поля!", QMessageBox::Yes);
        return;
    }
    if(EditMode)    {
        emit signalUpdateModel(ID,
                               ui->eName->text(),
                               ui->cbObject->currentText(),
                               ui->cbSystem->currentText(),
                               ui->eDescription->text());
    } else {
        emit signalAddModel(ui->eName->text(),
                            ui->cbObject->currentText(),
                            ui->cbSystem->currentText(),
                            ui->eDescription->text());
    }
    this->close();
}
