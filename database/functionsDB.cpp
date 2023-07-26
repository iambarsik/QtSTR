#include "form.h"
#include "ui_form.h"

void form::createDB()
{
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
                     "par1_type TEXT NOT NULL,"
                     "par2_type TEXT NOT NULL,"
                     "par1_text TEXT NOT NULL,"
                     "par2_text TEXT NOT NULL,"
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
    temp_query->bindValue(":name", "qint64");
    temp_query->exec();
    temp_query->prepare("INSERT INTO variable_types (name) "
                        "VALUES (:name)");
    temp_query->bindValue(":name", "double");
    temp_query->exec();
    temp_query->prepare("INSERT INTO variable_types (name) "
                        "VALUES (:name)");
    temp_query->bindValue(":name", "char");
    temp_query->exec();
}

void form::slotAddModel(model_struct mod)
{
    QSharedPointer<QSqlQuery> temp_query = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    temp_query->prepare("INSERT INTO models (name, object, system, description) "
                        "VALUES (:name, :object, :system, :description)");
    temp_query->bindValue(":name", mod.name);
    temp_query->bindValue(":object", mod.object);
    temp_query->bindValue(":system", mod.system);
    temp_query->bindValue(":description", mod.description);
    if(!temp_query->exec()) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось добавить модель!", QMessageBox::Yes);
        return;
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
}

void form::slotUpdateModel(model_struct mod)
{
    QSharedPointer<QSqlQuery> temp_query = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    temp_query->prepare("UPDATE models SET name = :name, object = :object, system = :system, description = :description "
                        "WHERE id = :id");
    temp_query->bindValue(":name", mod.name);
    temp_query->bindValue(":object", mod.object);
    temp_query->bindValue(":system", mod.system);
    temp_query->bindValue(":description", mod.description);
    temp_query->bindValue(":id", mod.id);
    if(!temp_query->exec()) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось обновить модель!", QMessageBox::Yes);
        return;
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
}

void form::slotAddFormat(format_struct form)
{
    QSharedPointer<QSqlQuery> temp_query = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    temp_query->prepare("INSERT INTO formats (name, title, object, system, description) "
                        "VALUES (:name, :title, :object, :system, :description)");
    temp_query->bindValue(":name", form.name);
    temp_query->bindValue(":title", form.title);
    temp_query->bindValue(":object", form.object);
    temp_query->bindValue(":system", form.system);
    temp_query->bindValue(":description", form.description);
    if(!temp_query->exec()) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось добавить формат!", QMessageBox::Yes);
        return;
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
}

void form::slotUpdateFormat(format_struct form)
{
    QSharedPointer<QSqlQuery> temp_query = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    temp_query->prepare("UPDATE formats SET name = :name, title = :title, object = :object, system = :system, description = :description "
                         "WHERE id = :id");
    temp_query->bindValue(":name", form.name);
    temp_query->bindValue(":title", form.title);
    temp_query->bindValue(":object", form.object);
    temp_query->bindValue(":system", form.system);
    temp_query->bindValue(":description", form.description);
    temp_query->bindValue(":id", form.id);
    if(!temp_query->exec()) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось обновить формат!", QMessageBox::Yes);
        return;
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
}

void form::slotAddCommand(command_struct com)
{
    QSharedPointer<QSqlQuery> temp_query = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    temp_query->prepare("INSERT INTO commands (name, par1_type, par2_type, par1_text, par2_text, model, description) "
                        "VALUES (:name, :par1_type, :par2_type, :par1_text, :par2_text, :model, :description)");
    temp_query->bindValue(":name", com.name);
    temp_query->bindValue(":par1_type", com.par1_type);
    temp_query->bindValue(":par2_type", com.par2_type);
    temp_query->bindValue(":par1_text", com.par1_text);
    temp_query->bindValue(":par2_text", com.par2_text);
    temp_query->bindValue(":model", com.model);
    temp_query->bindValue(":description", com.description);
    if(!temp_query->exec()) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось добавить команду!", QMessageBox::Yes);
        return;
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
}

void form::slotUpdateCommand(command_struct com)
{
    QSharedPointer<QSqlQuery> temp_query = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    temp_query->prepare("UPDATE commands SET name = :name, model = :model, description = :description "
                        "WHERE id = :id");
    temp_query->bindValue(":name", com.name);
    temp_query->bindValue(":model", com.model);
    temp_query->bindValue(":description", com.description);
    temp_query->bindValue(":id", com.id);
    if(!temp_query->exec()) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось обновить команду!", QMessageBox::Yes);
        return;
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
}

void form::slotAddVariable(variable_struct var)
{
    QSharedPointer<QSqlQuery> temp_query = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    temp_query->prepare("INSERT INTO variables (name, model, type, size, description) "
                        "VALUES (:name, :model, :type, :size, :description)");
    temp_query->bindValue(":name", var.name);
    temp_query->bindValue(":model", var.model);
    temp_query->bindValue(":type", var.type);
    temp_query->bindValue(":size", var.size);
    temp_query->bindValue(":description", var.description);
    if(!temp_query->exec()) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось добавить переменную!", QMessageBox::Yes);
        return;
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
}

void form::slotUpdateVariable(variable_struct var)
{
    QSharedPointer<QSqlQuery> temp_query = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    temp_query->prepare("UPDATE variables SET name = :name, model = :model, type = :type, size = :size, description = :description "
                        "WHERE id = :id");
    temp_query->bindValue(":name", var.name);
    temp_query->bindValue(":model", var.model);
    temp_query->bindValue(":type", var.type);
    temp_query->bindValue(":size", var.size);
    temp_query->bindValue(":description", var.description);
    temp_query->bindValue(":id", var.id);
    if(!temp_query->exec()) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось обновить переменную!", QMessageBox::Yes);
        return;
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
}

void form::slotSQLrequest(QString execute)
{
    QSharedPointer<QSqlQuery> q = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
    if(!q->exec(execute)) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось выполнить запрос!", QMessageBox::Yes);
        return;
    }
    this->on_comboBox_tables_activated(ui->comboBox_tables->currentIndex());
}

void form::log(QString message)
{
    qDebug() << message;
    QString filename = QDir::currentPath() + "/log/" + m_log_name;
    QFile file(m_log_name);
    file.open(QIODevice::Append);
    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForName("UTF-8"));
    QString st = QTime::currentTime().toString();
    out << QString("%1 : %2").arg(st).arg(message) << "\n";

    file.close();
}
