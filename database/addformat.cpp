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

void addformat::setCurrentData(QString name, QString title, QString object, QString system, QString description)
{
    ui->eName->setText(name);
    ui->eTitle->setText(title);
    ui->cbObject->setCurrentText(object);
    ui->cbSystem->setCurrentText(system);
    ui->eDescription->setText(description);
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
    if(EditMode)    {
        emit signalUpdateFormat(ID,
                                ui->eName->text(),
                                ui->eTitle->text(),
                                ui->cbObject->currentText(),
                                ui->cbSystem->currentText(),
                                ui->eDescription->text());
    } else {
        emit signalAddFormat(ui->eName->text(),
                             ui->eTitle->text(),
                             ui->cbObject->currentText(),
                             ui->cbSystem->currentText(),
                             ui->eDescription->text());
    }
    this->close();
}
