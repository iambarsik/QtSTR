#include "form.h"
#include "ui_form.h"

#include <QFile>

form::form(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::form)
{
    ui->setupUi(this);

    QDate log_date = QDate::currentDate();
    m_log_name = QString("log_%1-%2-%3").arg(log_date.day()).arg(log_date.month()).arg(log_date.year());
    log("================================================");

    sProjectDir = "generated_data";

    sDB_tag = "/*<::db_generated::>*/";

    QString sDB_NAME = "./database.db";

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(sDB_NAME);

    bool bCreateDB = false;
    if(!QFile::exists(sDB_NAME))    {
        bCreateDB = true;
    }

    if(db.open())    {
        if(bCreateDB == true)   {
            log("SYS :: can't open database...");
            log("SYS :: creating database manualy...");
            try {
                createDB();
                log("SYS :: database is created...");
            } catch(QException *e) {
                log(e->what());
            }
        }
    } else {
        log("SYS :: opening DB error");
        QMessageBox::warning(this,"ОШИБКА","Невозможно открыть базу данных",QMessageBox::Ok);
        QApplication::exit(-1);
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

    connect(&addModelForm, SIGNAL(signalAddModel(model_struct)),
            this, SLOT(slotAddModel(model_struct)));
    connect(&addModelForm, SIGNAL(signalUpdateModel(model_struct)),
            this, SLOT(slotUpdateModel(model_struct)));

    connect(&addFormatForm, SIGNAL(signalAddFormat(format_struct)),
            this, SLOT(slotAddFormat(format_struct)));
    connect(&addFormatForm, SIGNAL(signalUpdateFormat(format_struct)),
            this, SLOT(slotUpdateFormat(format_struct)));

    connect(&addCommandForm, SIGNAL(signalAddCommand(command_struct)),
            this, SLOT(slotAddCommand(command_struct)));
    connect(&addCommandForm, SIGNAL(signalUpdateCommand(command_struct)),
            this, SLOT(slotUpdateCommand(command_struct)));

    connect(&addVariableForm, SIGNAL(signalAddVariable(variable_struct)),
            this, SLOT(slotAddVariable(variable_struct)));
    connect(&addVariableForm, SIGNAL(signalUpdateVariable(variable_struct)),
            this, SLOT(slotUpdateVariable(variable_struct)));

    connect(&sqlForm,SIGNAL(signalSQLrequest(QString)),this,SLOT(slotSQLrequest(QString)));

    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());

    this->setWindowIcon(QIcon(":/images/ico.ico"));
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
                         QString label7, QString data7,
                         QString description)
{
    ui->label1info->setText(label1); ui->label1data->setText(data1);
    ui->label2info->setText(label2); ui->label2data->setText(data2);
    ui->label3info->setText(label3); ui->label3data->setText(data3);
    ui->label4info->setText(label4); ui->label4data->setText(data4);
    ui->label5info->setText(label5); ui->label5data->setText(data5);
    ui->label6info->setText(label6); ui->label6data->setText(data6);
    ui->label7info->setText(label7); ui->label7data->setText(data7);
    ui->labelDescription->setText(description);
}

void form::on_bAdd_clicked()
{
    switch(currentScreen) {
        case Screen::model: {
            model_struct mod;
            addModelForm.setCurrentData({ -1, "", "", "", ""});
            addModelForm.setData(getNamesListFromTable("objects"),
                                 getNamesListFromTable("systems"));
            addModelForm.setModal(true);
            addModelForm.show();
        } break;
        case Screen::format: {
            addFormatForm.setCurrentData({ -1, "", "", "", "", ""});
            addFormatForm.setData(getNamesListFromTable("objects"),
                                 getNamesListFromTable("systems"));
            addFormatForm.setModal(true);
            addFormatForm.show();
        } break;
        case Screen::variable: {
            addVariableForm.setCurrentData({ -1, "", "", "", 0, ""});
            addVariableForm.setData(getNamesListFromTable("models"),
                                    getNamesListFromTable("variable_types"));
            addVariableForm.setModal(true);
            addVariableForm.show();
        } break;
        case Screen::command: {
            addCommandForm.setCurrentData({ -1, "", "", "", "", "", "", ""});
            addCommandForm.setData(getNamesListFromTable("models"),getNamesListFromTable("variable_types"));
            addCommandForm.setModal(true);
            addCommandForm.show();
        } break;
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
                              "","",
                              q->value(5).toString());
                currentEditID = q->value(0).toInt();
            }
        } break;
        case Screen::command: {
            QString sExecute = "SELECT id, name, par1_type, par2_type, par1_text, par2_text, model, description FROM commands WHERE name = '" + sName + "'";
            QSharedPointer<QSqlQuery> q = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
            q->exec(sExecute);
            QSqlRecord r = q->record();
            while(q->next()) {
            setWidgetInfo("Имя команды: ",q->value(1).toString(),
                          "Модель-обработчик: ",q->value(6).toString(),
                          "ID: ",q->value(0).toString(),
                          QString("Пар.1: %1").arg(q->value(2).toString()),QString("%1").arg(q->value(4).toString()),
                          QString("Пар.2: %1").arg(q->value(3).toString()),QString("%1").arg(q->value(5).toString()),
                          "","",
                          "","",
                          q->value(7).toString());
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
        log("Error deleting from table");
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
                addModelForm.setCurrentData({q->value(0).toInt(),
                                             q->value(1).toString(),
                                             q->value(2).toString(),
                                             q->value(3).toString(),
                                             q->value(4).toString()});
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
                addFormatForm.setCurrentData({q->value(0).toInt(),
                                              q->value(1).toString(),
                                              q->value(2).toString(),
                                              q->value(3).toString(),
                                              q->value(4).toString(),
                                              q->value(5).toString()});
            }
            addFormatForm.setModal(true);
            addFormatForm.show();
        } break;
        case Screen::variable: {
            addVariableForm.setData(getNamesListFromTable("models"),
                                    getNamesListFromTable("variable_types"),true,currentEditID);
            QString sName = ui->listWidget->currentItem()->text();
            QString sExecute = "SELECT id, name, model, type, size, description FROM variables WHERE name = '" + sName + "'";
            QSharedPointer<QSqlQuery> q = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
            q->exec(sExecute);
            while(q->next()) {
                addVariableForm.setCurrentData({q->value(0).toInt(),
                                                q->value(1).toString(),
                                                q->value(2).toString(),
                                                q->value(3).toString(),
                                                q->value(4).toUInt(),
                                                q->value(5).toString()});

            }
            addVariableForm.setModal(true);
            addVariableForm.show();
        } break;
        case Screen::command:
            addCommandForm.setData(getNamesListFromTable("models"),getNamesListFromTable("variable_types"),true,currentEditID);
            QString sName = ui->listWidget->currentItem()->text();
            QString sExecute = "SELECT id, name, par1_type, par2_type, par1_text, par1_text, model, description FROM commands WHERE name = '" + sName + "'";
            QSharedPointer<QSqlQuery> q = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
            q->exec(sExecute);
            while(q->next()) {
                addCommandForm.setCurrentData({q->value(0).toInt(),
                                               q->value(1).toString(),
                                               q->value(2).toString(),
                                               q->value(3).toString(),
                                               q->value(4).toString(),
                                               q->value(5).toString(),
                                               q->value(6).toString(),
                                               q->value(7).toString()});
            }
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
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", "Построить исходные коды?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
        return;

    try {
        generateCore();
        generateModels();
        generateFormats();
        generateManager();
        generateGlobalProject();
    } catch (QException *e) {
        log(e->what());
    }

    QMessageBox::warning(this,"Сообщение","Генерация исходных кодов завершена",QMessageBox::Ok);
}
