#include "addformat.h"
#include "ui_addformat.h"

#include <QMessageBox>

addformat::addformat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addformat)
{
    ui->setupUi(this);
}

addformat::~addformat()
{
    delete ui;
}

void addformat::setData(const QStringList Objects, const QStringList Systems, bool EditMode, int id)
{
    ui->cbObject->clear();
    ui->cbSystem->clear();
    ui->cbObject->addItems(Objects);
    ui->cbSystem->addItems(Systems);
    this->EditMode = EditMode;
    this->ID = id;
    if(EditMode)
        setWindowTitle("Редактирование формата");
    else
        setWindowTitle("Добавление формата");
}

void addformat::setCurrentData(format_struct form)
{
    ui->eName->setText(form.name);
    ui->eTitle->setText(form.title);
    ui->cbObject->setCurrentText(form.object);
    ui->cbSystem->setCurrentText(form.system);
    ui->eDescription->setText(form.description);
}

void addformat::on_bCancel_clicked()
{
    this->close();
}

void addformat::on_bAdd_clicked()
{
    if(ui->eName->text().isEmpty() ||
       ui->eTitle->text().isEmpty() ||
       ui->eDescription->text().isEmpty() ||
       ui->cbObject->currentText().isEmpty()||
       ui->cbSystem->currentText().isEmpty())
    {
        QMessageBox::warning(this, "Ошибка!", "Заполните все поля!", QMessageBox::Yes);
        return;
    }
    format_struct form;
    form.name = ui->eName->text();
    form.title = ui->eTitle->text();
    form.object = ui->cbObject->currentText();
    form.system = ui->cbSystem->currentText();
    form.description = ui->eDescription->text();
    if(EditMode)    {
        form.id = ID;
        emit signalUpdateFormat(form);
    } else {
        form.id = -1;
        emit signalAddFormat(form);
    }
    this->close();
}
