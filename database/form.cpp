#include "form.h"
#include "ui_form.h"

#include <QFile>
#include <QMessageBox>
#include <QSharedPointer>

form::form(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::form)
{
    ui->setupUi(this);

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
            QSharedPointer<QSqlQuery> temp_query = QSharedPointer<QSqlQuery>(new QSqlQuery(db));

            temp_query->exec("CREATE TABLE variable_types("
                             "id INTEGER NOT NULL UNIQUE,"
                             "name TEXT NOT NULL UNIQUE,"
                             "PRIMARY KEY(id AUTOINCREMENT)"
                             ");");

            temp_query->exec("CREATE TABLE systems("
                             "id INTEGER NOT NULL UNIQUE,"
                             "name TEXT NOT NULL UNIQUE,"
                             "PRIMARY KEY(id AUTOINCREMENT)"
                             ");");

            temp_query->exec("CREATE TABLE objects("
                             "id INTEGER NOT NULL UNIQUE,"
                             "name TEXT NOT NULL UNIQUE,"
                             "PRIMARY KEY(id AUTOINCREMENT)"
                             ");");

            temp_query->exec("CREATE TABLE models("
                             "id INTEGER NOT NULL UNIQUE,"
                             "name TEXT NOT NULL UNIQUE,"
                             "object TEXT NOT NULL,"
                             "system TEXT NOT NULL,"
                             "description TEXT NOT NULL,"
                             "PRIMARY KEY(id AUTOINCREMENT)"
                             ");");

            temp_query->exec("CREATE TABLE formats("
                             "id INTEGER NOT NULL UNIQUE,"
                             "name TEXT NOT NULL UNIQUE,"
                             "title TEXT NOT NULL,"
                             "object TEXT NOT NULL,"
                             "system TEXT NOT NULL,"
                             "description TEXT NOT NULL,"
                             "PRIMARY KEY(id AUTOINCREMENT)"
                             ");");

            temp_query->exec("CREATE TABLE variables("
                             "id INTEGER NOT NULL UNIQUE,"
                             "name TEXT NOT NULL UNIQUE,"
                             "type TEXT NOT NULL,"
                             "size INTEGER NOT NULL,"
                             "model TEXT NOT NULL,"
                             "description TEXT NOT NULL,"
                             "PRIMARY KEY(id AUTOINCREMENT)"
                             ");");

            temp_query->exec("CREATE TABLE commands("
                             "id INTEGER NOT NULL UNIQUE,"
                             "name TEXT NOT NULL UNIQUE,"
                             "model TEXT NOT NULL,"
                             "description TEXT NOT NULL,"
                             "PRIMARY KEY(id AUTOINCREMENT)"
                             ");");


            temp_query->prepare("INSERT INTO systems (name) "
                                "VALUES (:name)");
            temp_query->bindValue(":name", "СЖО");
            temp_query->exec();
            temp_query->prepare("INSERT INTO systems (name) "
                                "VALUES (:name)");
            temp_query->bindValue(":name", "СОТР");
            temp_query->exec();
            temp_query->prepare("INSERT INTO systems (name) "
                                "VALUES (:name)");
            temp_query->bindValue(":name", "СУБА");
            temp_query->exec();

            temp_query->prepare("INSERT INTO objects (name) "
                                "VALUES (:name)");
            temp_query->bindValue(":name", "СМ");
            temp_query->exec();
            temp_query->prepare("INSERT INTO objects (name) "
                                "VALUES (:name)");
            temp_query->bindValue(":name", "ФГБ");
            temp_query->exec();
            temp_query->prepare("INSERT INTO objects (name) "
                                "VALUES (:name)");
            temp_query->bindValue(":name", "МИМ-1");
            temp_query->exec();
            temp_query->prepare("INSERT INTO objects (name) "
                                "VALUES (:name)");
            temp_query->bindValue(":name", "МИМ-2");
            temp_query->exec();
            temp_query->prepare("INSERT INTO objects (name) "
                                "VALUES (:name)");
            temp_query->bindValue(":name", "МЛМ");
            temp_query->exec();
            temp_query->prepare("INSERT INTO objects (name) "
                                "VALUES (:name)");
            temp_query->bindValue(":name", "НЭМ");
            temp_query->exec();
            temp_query->prepare("INSERT INTO variable_types (name) "
                                "VALUES (:name)");
            temp_query->bindValue(":name", "qint32");
            temp_query->exec();
            temp_query->prepare("INSERT INTO variable_types (name) "
                                "VALUES (:name)");
            temp_query->bindValue(":name", "double");
            temp_query->exec();
        }

    } else {

        qDebug() << "SYS :: opening DB error";
    }

    m_query = new QSqlQuery(db);
    m_model = new QSqlTableModel(this,db);

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


    m_model->setTable(ui->comboBox_tables->itemText(0));
    m_model->select();
    //ui->tableView->setModel(m_model);

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
    //m_model->setTable(sTable);
    m_model->setTable(ui->comboBox_tables->itemText(index));
    m_model->select();

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
        ui->bRemove->setVisible(false);
    } else {
        ui->bRemove->setVisible(true);
    }
    currentEditID = -1;
}

void form::slotAddModel(QString name, QString object, QString system, QString description)
{
    QSharedPointer<QSqlQuery> temp_query = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    temp_query->prepare("INSERT INTO models (name, object, system, description) "
                        "VALUES (:name, :object, :system, :description)");
    temp_query->bindValue(":name", name);
    temp_query->bindValue(":object", object);
    temp_query->bindValue(":system", system);
    temp_query->bindValue(":description", description);
    if(!temp_query->exec()) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось добавить модель!", QMessageBox::Yes);
        return;
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
}

void form::slotUpdateModel(int id, QString name, QString object, QString system, QString description)
{
    QSharedPointer<QSqlQuery> temp_query = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    temp_query->prepare("UPDATE models SET name = :name, object = :object, system = :system, description = :description "
                        "WHERE id = :id");
    temp_query->bindValue(":name", name);
    temp_query->bindValue(":object", object);
    temp_query->bindValue(":system", system);
    temp_query->bindValue(":description", description);
    temp_query->bindValue(":id", id);
    if(!temp_query->exec()) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось обновить модель!", QMessageBox::Yes);
        return;
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
}

void form::slotAddFormat(QString name, QString title, QString object, QString system, QString description)
{
    QSharedPointer<QSqlQuery> temp_query = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    temp_query->prepare("INSERT INTO formats (name, title, object, system, description) "
                        "VALUES (:name, :title, :object, :system, :description)");
    temp_query->bindValue(":name", name);
    temp_query->bindValue(":title", title);
    temp_query->bindValue(":object", object);
    temp_query->bindValue(":system", system);
    temp_query->bindValue(":description", description);
    if(!temp_query->exec()) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось добавить формат!", QMessageBox::Yes);
        return;
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
}

void form::slotUpdateFormat(int id, QString name, QString title, QString object, QString system, QString description)
{
    QSharedPointer<QSqlQuery> temp_query = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    temp_query->prepare("UPDATE formats SET name = :name, title = :title, object = :object, system = :system, description = :description "
                         "WHERE id = :id");
    temp_query->bindValue(":name", name);
    temp_query->bindValue(":title", title);
    temp_query->bindValue(":object", object);
    temp_query->bindValue(":system", system);
    temp_query->bindValue(":description", description);
    temp_query->bindValue(":id", id);
    if(!temp_query->exec()) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось обновить формат!", QMessageBox::Yes);
        return;
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
}

void form::slotAddCommand(QString name, QString model, QString description)
{
    QSharedPointer<QSqlQuery> temp_query = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    temp_query->prepare("INSERT INTO commands (name, model, description) "
                        "VALUES (:name, :model, :description)");
    temp_query->bindValue(":name", name);
    temp_query->bindValue(":model", model);
    temp_query->bindValue(":description", description);
    if(!temp_query->exec()) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось добавить команду!", QMessageBox::Yes);
        return;
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
}

void form::slotUpdateCommand(int id, QString name, QString model, QString description)
{
    QSharedPointer<QSqlQuery> temp_query = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    temp_query->prepare("UPDATE commands SET name = :name, model = :model, description = :description "
                        "WHERE id = :id");
    temp_query->bindValue(":name", name);
    temp_query->bindValue(":model", model);
    temp_query->bindValue(":description", description);
    temp_query->bindValue(":id", id);
    if(!temp_query->exec()) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось обновить команду!", QMessageBox::Yes);
        return;
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
}

void form::slotAddVariable(QString name, QString model, QString type, uint size, QString description)
{
    QSharedPointer<QSqlQuery> temp_query = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    temp_query->prepare("INSERT INTO variables (name, model, type, size, description) "
                        "VALUES (:name, :model, :type, :size, :description)");
    temp_query->bindValue(":name", name);
    temp_query->bindValue(":model", model);
    temp_query->bindValue(":type", type);
    temp_query->bindValue(":size", size);
    temp_query->bindValue(":description", description);
    if(!temp_query->exec()) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось добавить переменную!", QMessageBox::Yes);
        return;
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
}

void form::slotUpdateVariable(int id, QString name, QString model, QString type, uint size, QString description)
{
    QSharedPointer<QSqlQuery> temp_query = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    temp_query->prepare("UPDATE variables SET name = :name, model = :model, type = :type, size = :size, description = :description "
                        "WHERE id = :id");
    temp_query->bindValue(":name", name);
    temp_query->bindValue(":model", model);
    temp_query->bindValue(":type", type);
    temp_query->bindValue(":size", size);
    temp_query->bindValue(":description", description);
    temp_query->bindValue(":id", id);
    if(!temp_query->exec()) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось обновить переменную!", QMessageBox::Yes);
        return;
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
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
            addModelForm.setData(getNamesListFromTable("objects"),
                                 getNamesListFromTable("systems"));
            addModelForm.setModal(true);
            addModelForm.show();
        break;
        case Screen::format:
            addFormatForm.setData(getNamesListFromTable("objects"),
                                 getNamesListFromTable("systems"));
            addFormatForm.setModal(true);
            addFormatForm.show();
        break;
        case Screen::variable:
            addVariableForm.setData(getNamesListFromTable("models"),
                                    getNamesListFromTable("variable_types"));
            addVariableForm.setModal(true);
            addVariableForm.show();
        break;
        case Screen::command:
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
    q->exec(sExecute);

    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
    ui->widgetInfo->setVisible(false);
    if(ui->listWidget->count() == 0)    {
        ui->bRemove->setVisible(false);
    } else {
        ui->bRemove->setVisible(true);
    }

}


void form::on_bEdit_clicked()
{
    switch(currentScreen) {
        case Screen::model:
            addModelForm.setData(getNamesListFromTable("objects"),
                                 getNamesListFromTable("systems"),true,currentEditID);
            addModelForm.setModal(true);
            addModelForm.show();
        break;
        case Screen::format:
            addFormatForm.setData(getNamesListFromTable("objects"),
                                 getNamesListFromTable("systems"),true,currentEditID);
            addFormatForm.setModal(true);
            addFormatForm.show();
        break;
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
