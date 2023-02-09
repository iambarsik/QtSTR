#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase(("QSQLITE"));
    db.setDatabaseName("./database.db");
    if(db.open())    {
        qDebug() << "Database is opened";
    } else {
        qDebug() << "Can't open database";
    }
    query = new QSqlQuery(db);
    //query->exec("CREATE TABLE Commands(command_name TEXT, command_owner TEXT, command_description TEXT);");
    table_model = new QSqlTableModel(this,db);

    table_model->setTable("command");
    table_model->select();
    ui->tableView->setModel(table_model);

    ui->comboBox->addItem("command");
    ui->comboBox->addItem("variable");
    ui->comboBox->addItem("system");
    ui->comboBox->addItem("format");
    ui->comboBox->addItem("model");
    ui->comboBox->addItem("type");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonAdd_clicked()
{
    table_model->insertRow(table_model->rowCount());
}

void MainWindow::on_pushButtonRemove_clicked()
{
    if(currentRow != -1)    {
        table_model->removeRow(currentRow);
    }
    ui->tableView->update();

}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    currentRow = index.row();
}

void MainWindow::on_pushButton_clicked()
{
    ui->textEdit->clear();
    QString command = ui->lineEdit->text();
    QSqlQuery q(command);
    QSqlRecord r = q.record();
    while(q.next()) {
        ui->textEdit->append(q.value(0).toString());
    }
}

void MainWindow::on_comboBox_activated(int index)
{
    switch(index)   {
        case 0:
            table_model->setTable("command");
            table_model->select();
            ui->tableView->setModel(table_model);
        break;
        case 1:
            table_model->setTable("variable");
            table_model->select();
            ui->tableView->setModel(table_model);

        break;
        case 2:
            table_model->setTable("system");
            table_model->select();
            ui->tableView->setModel(table_model);
        break;
        case 3:
            table_model->setTable("format");
            table_model->select();
            ui->tableView->setModel(table_model);
        break;
        case 4:
            table_model->setTable("model");
            table_model->select();
            ui->tableView->setModel(table_model);
        break;
        case 5:
            table_model->setTable("type");
            table_model->select();
            ui->tableView->setModel(table_model);
        break;
    }
}
