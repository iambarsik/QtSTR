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
    if(EditMode)
        setWindowTitle("Редактирование модели");
    else
        setWindowTitle("Добавление модели");
}

void addmodel::setCurrentData(model_struct mod)
{
    ui->eName->setText(mod.name);
    ui->cbObject->setCurrentText(mod.object);
    ui->cbSystem->setCurrentText(mod.system);
    ui->eDescription->setText(mod.description);
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
    model_struct model;
    model.name = ui->eName->text();
    model.object = ui->cbObject->currentText();
    model.system = ui->cbSystem->currentText();
    model.description = ui->eDescription->text();
    if(EditMode)    {
        model.id = ID;
        emit signalUpdateModel(model);
    } else {
        model.id = -1;
        emit signalAddModel(model);
    }
    this->close();
}
