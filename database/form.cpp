#include "form.h"
#include "ui_form.h"

#include <QFile>

form::form(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::form)
{
    ui->setupUi(this);

    sProjectDir = "generated_data";
    sDB_tag = "/*<::db_generated::>*/";

    QString sDB_NAME = "./database.db";

    db = QSqlDatabase::addDatabase(("QSQLITE"));
    db.setDatabaseName(sDB_NAME);

    bool bCreateDB = false;
    if(!QFile::exists(sDB_NAME))    {
        bCreateDB = true;
    }

    if(db.open())    {
        if(bCreateDB == true)   {
            qDebug() << "SYS :: can't open database...";
            qDebug() << "SYS :: creating database manualy...";
            try {
                createDB();
                qDebug() << "SYS :: database is created...";
            } catch(QException *e) {
                qDebug() << e->what();
            }
        }
    } else {

        qDebug() << "SYS :: opening DB error";
    }
    m_query = new QSqlQuery(db);

/*
    QSqlQuery q("SELECT name FROM sqlite_master WHERE type='table'");
    QSqlRecord r = q.record();
    while(q.next()) {
        QString str = q.value(0).toString();
        if(str != "sqlite_sequence")
            ui->comboBox_tables->addItem(str);
    }
*/

    ui->comboBox_tables->addItem("Модели");
    ui->comboBox_tables->addItem("Форматы");
    ui->comboBox_tables->addItem("Переменные");
    ui->comboBox_tables->addItem("Команды");

    connect(&addModelForm, SIGNAL(signalAddModel(QString,QString,QString,QString)),
            this, SLOT(slotAddModel(QString,QString,QString,QString)));
    connect(&addModelForm, SIGNAL(signalUpdateModel(int,QString,QString,QString,QString)),
            this, SLOT(slotUpdateModel(int,QString,QString,QString,QString)));

    connect(&addFormatForm, SIGNAL(signalAddFormat(QString,QString,QString,QString,QString)),
            this, SLOT(slotAddFormat(QString,QString,QString,QString,QString)));
    connect(&addFormatForm, SIGNAL(signalUpdateFormat(int,QString,QString,QString,QString,QString)),
            this, SLOT(slotUpdateFormat(int,QString,QString,QString,QString,QString)));

    connect(&addCommandForm, SIGNAL(signalAddCommand(QString,QString,QString)),
            this, SLOT(slotAddCommand(QString,QString,QString)));
    connect(&addCommandForm, SIGNAL(signalUpdateCommand(int,QString,QString,QString)),
            this, SLOT(slotUpdateCommand(int,QString,QString,QString)));

    connect(&addVariableForm, SIGNAL(signalAddVariable(QString,QString,QString,uint,QString)),
            this, SLOT(slotAddVariable(QString,QString,QString,uint,QString)));
    connect(&addVariableForm, SIGNAL(signalUpdateVariable(int,QString,QString,QString,uint,QString)),
            this, SLOT(slotUpdateVariable(int,QString,QString,QString,uint,QString)));

    connect(&sqlForm,SIGNAL(signalSQLrequest(QString)),this,SLOT(slotSQLrequest(QString)));

    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
}

form::~form()
{
    delete ui;
}

void form::on_comboBox_tables_activated(int index)
{
    QString sTable;
    switch (index) {
        case 0: sTable = "models"; currentScreen = Screen::model; break;
        case 1: sTable = "formats"; currentScreen = Screen::format; break;
        case 2: sTable = "variables"; currentScreen = Screen::variable; break;
        case 3: sTable = "commands"; currentScreen = Screen::command; break;
    }

    ui->listWidget->clear();

    QString sExecute = "SELECT name FROM " + sTable;
    QSqlQuery q(sExecute);
    QSqlRecord r = q.record();
    while(q.next()) {
        QString str = q.value(0).toString();
        ui->listWidget->insertItem(ui->listWidget->count(),str);
    }
    ui->widgetInfo->setVisible(false);
    if(ui->listWidget->count() == 0)    {
        ui->bEdit->setEnabled(false);
        ui->bRemove->setEnabled(false);
    } else {
        ui->bEdit->setEnabled(true);
        ui->bRemove->setEnabled(true);
    }
    currentEditID = -1;
}

QStringList form::getNamesListFromTable(QString tablename)
{
    QStringList data;
    QString sExecute = "SELECT name FROM " + tablename;
    QSqlQuery q(sExecute);
    QSqlRecord r = q.record();
    while(q.next()) {
        data.append(q.value(0).toString());
    }
    return data;
}

QString form::getCellFromTable(QString target, QString tablename, QString key, QString value)
{
    QString data;
    QString sExecute = QString("SELECT %1 FROM %2 WHERE %3 = '%4'").arg(target).arg(tablename).arg(key).arg(value);
    QSqlQuery q(sExecute);
    QSqlRecord r = q.record();
    while(q.next()) {
        data = q.value(0).toString();
    }
    return data;
}

void form::setWidgetInfo(QString label1, QString data1,
                         QString label2, QString data2,
                         QString label3, QString data3,
                         QString label4, QString data4,
                         QString label5, QString data5,
                         QString label6, QString data6,
                         QString description)
{
    ui->label1info->setText(label1); ui->label1data->setText(data1);
    ui->label2info->setText(label2); ui->label2data->setText(data2);
    ui->label3info->setText(label3); ui->label3data->setText(data3);
    ui->label4info->setText(label4); ui->label4data->setText(data4);
    ui->label5info->setText(label5); ui->label5data->setText(data5);
    ui->label6info->setText(label6); ui->label6data->setText(data6);
    ui->labelDescription->setText(description);
}

void form::on_bAdd_clicked()
{
    switch(currentScreen) {
        case Screen::model:
            addModelForm.setCurrentData("","","","");
            addModelForm.setData(getNamesListFromTable("objects"),
                                 getNamesListFromTable("systems"));
            addModelForm.setModal(true);
            addModelForm.show();
        break;
        case Screen::format:
            addFormatForm.setCurrentData("","","","","");
            addFormatForm.setData(getNamesListFromTable("objects"),
                                 getNamesListFromTable("systems"));
            addFormatForm.setModal(true);
            addFormatForm.show();
        break;
        case Screen::variable:
            addVariableForm.setCurrentData("","","",0,"");
            addVariableForm.setData(getNamesListFromTable("models"),
                                    getNamesListFromTable("variable_types"));
            addVariableForm.setModal(true);
            addVariableForm.show();
        break;
        case Screen::command:
            addCommandForm.setCurrentData("","","");
            addCommandForm.setData(getNamesListFromTable("models"));
            addCommandForm.setModal(true);
            addCommandForm.show();
        break;
    }
}

void form::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString sName = item->text();
    switch (currentScreen) {
        case Screen::model: {
            QString sExecute = "SELECT id, name, object, system, description FROM models WHERE name = '" + sName + "'";
            QSharedPointer<QSqlQuery> q = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
            q->exec(sExecute);
            QSqlRecord r = q->record();
            while(q->next()) {
                setWidgetInfo("Имя модели: ",q->value(1).toString(),
                              "Изделие: ",q->value(2).toString(),
                              "Система: ",q->value(3).toString(),
                              "ID: ",q->value(0).toString(),
                              "","",
                              "","",
                              q->value(4).toString());
                currentEditID = q->value(0).toInt();
            }
        } break;
        case Screen::format: {
            QString sExecute = "SELECT id, name, title, object, system, description FROM formats WHERE name = '" + sName + "'";
            QSharedPointer<QSqlQuery> q = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
            q->exec(sExecute);
            QSqlRecord r = q->record();
            while(q->next()) {
                setWidgetInfo("Имя формата: ",q->value(1).toString(),
                              "Имя в списке: ",q->value(2).toString(),
                              "Изделие: ",q->value(3).toString(),
                              "Система: ",q->value(4).toString(),
                              "ID: ",q->value(0).toString(),
                              "","",
                              q->value(5).toString());
                currentEditID = q->value(0).toInt();
            }
        } break;
        case Screen::variable: {
            QString sExecute = "SELECT id, name, model, type, size, description FROM variables WHERE name = '" + sName + "'";
            QSharedPointer<QSqlQuery> q = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
            q->exec(sExecute);
            QSqlRecord r = q->record();
            while(q->next()) {
                setWidgetInfo("Имя переменной: ",q->value(1).toString(),
                              "Модель-обработчик: ",q->value(2).toString(),
                              "Тип переменной: ",q->value(3).toString(),
                              "Размерность: ",q->value(4).toString(),
                              "ID: ",q->value(0).toString(),
                              "","",
                              q->value(5).toString());
                currentEditID = q->value(0).toInt();
            }
        } break;
        case Screen::command: {
            QString sExecute = "SELECT id, name, model, description FROM commands WHERE name = '" + sName + "'";
            QSharedPointer<QSqlQuery> q = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
            q->exec(sExecute);
            QSqlRecord r = q->record();
            while(q->next()) {
            setWidgetInfo("Имя команды: ",q->value(1).toString(),
                          "Модель-обработчик: ",q->value(2).toString(),
                          "ID: ",q->value(0).toString(),
                          "","",
                          "","",
                          "","",
                          q->value(3).toString());
            currentEditID = q->value(0).toInt();
            }
        } break;
    }
    ui->widgetInfo->setVisible(true);
}

void form::on_bRemove_clicked()
{
    if(ui->listWidget->currentRow() < 0)    {
        QMessageBox::warning(this, "Внимание!", "Не выбран ни один объект!",
                                      QMessageBox::Yes);
        return;
    }

    QString sName = ui->listWidget->currentItem()->text();
    QString sTable;
    QString sExecute;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", "Удалить " + sName + " из базы данных?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
        return;

    switch (currentScreen) {
        case Screen::model: sTable = "models"; break;
        case Screen::format: sTable = "formats"; break;
        case Screen::variable: sTable = "variables"; break;
        case Screen::command: sTable = "commands"; break;
    }

    sExecute = "DELETE FROM " + sTable + " WHERE name = '" + sName + "'";
    QSharedPointer<QSqlQuery> q = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    if(!q->exec(sExecute))  {
        qDebug() << "Error deleting from table";
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
    ui->widgetInfo->setVisible(false);
    if(ui->listWidget->count() == 0)    {
        ui->bEdit->setEnabled(false);
        ui->bRemove->setEnabled(false);
    } else {
        ui->bEdit->setEnabled(true);
        ui->bRemove->setEnabled(true);
    }
}

void form::on_bEdit_clicked()
{
    if(ui->listWidget->currentRow() < 0)    {
        QMessageBox::warning(this, "Внимание!", "Не выбран ни один объект!",
                                      QMessageBox::Yes);
        return;
    }
    switch(currentScreen) {
        case Screen::model: {
            addModelForm.setData(getNamesListFromTable("objects"),
                                 getNamesListFromTable("systems"),true,currentEditID);
            QString sName = ui->listWidget->currentItem()->text();
            QString sExecute = "SELECT id, name, object, system, description FROM models WHERE name = '" + sName + "'";
            QSharedPointer<QSqlQuery> q = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
            q->exec(sExecute);
            while(q->next()) {
                addModelForm.setCurrentData(q->value(1).toString(),
                                            q->value(2).toString(),
                                            q->value(3).toString(),
                                            q->value(4).toString());
            }
            addModelForm.setModal(true);
            addModelForm.show();
        } break;
        case Screen::format: {
            addFormatForm.setData(getNamesListFromTable("objects"),
                                 getNamesListFromTable("systems"),true,currentEditID);
            QString sName = ui->listWidget->currentItem()->text();
            QString sExecute = "SELECT id, name, title, object, system, description FROM formats WHERE name = '" + sName + "'";
            QSharedPointer<QSqlQuery> q = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
            q->exec(sExecute);
            while(q->next()) {
                addFormatForm.setCurrentData(q->value(1).toString(),
                                            q->value(2).toString(),
                                            q->value(3).toString(),
                                            q->value(4).toString(),
                                            q->value(5).toString());
            }
            addFormatForm.setModal(true);
            addFormatForm.show();
        } break;
        case Screen::variable:
            addVariableForm.setData(getNamesListFromTable("models"),
                                    getNamesListFromTable("variable_types"),true,currentEditID);
            addVariableForm.setModal(true);
            addVariableForm.show();
        break;
        case Screen::command:
            addCommandForm.setData(getNamesListFromTable("models"),true,currentEditID);
            addCommandForm.setModal(true);
            addCommandForm.show();
        break;
    }
}

void form::on_action_SQL_triggered()
{
    sqlForm.setModal(true);
    sqlForm.show();
}

void form::on_action_generate_triggered()
{
    try {
        generateCore();
    } catch (QException *e) {
        qDebug() << e->what();
    }
}
