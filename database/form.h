#ifndef FORM_H
#define FORM_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QStringList>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QSharedPointer>
#include <QException>
#include <QFile>
#include <QStringList>

#include "addmodel.h"
#include "addformat.h"
#include "addcommand.h"
#include "addvariable.h"
#include "sqlform.h"
#include "data_struct.h"

enum Screen {
    model,
    format,
    variable,
    command
};

QT_BEGIN_NAMESPACE
namespace Ui { class form; }
QT_END_NAMESPACE

class form : public QMainWindow
{
    Q_OBJECT

public:
    form(QWidget *parent = nullptr);
    ~form();

private slots:
    void on_comboBox_tables_activated(int index);

    void slotAddModel(model_struct mod);
    void slotUpdateModel(model_struct mod);
    void slotAddFormat(format_struct form);
    void slotUpdateFormat(format_struct form);
    void slotAddCommand(command_struct com);
    void slotUpdateCommand(command_struct com);
    void slotAddVariable(variable_struct var);
    void slotUpdateVariable(variable_struct var);
    void slotSQLrequest(QString execute);

    void log(QString message);

    void on_bAdd_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_bRemove_clicked();


    void on_bEdit_clicked();

    void on_action_SQL_triggered();

    void on_action_generate_triggered();

private:
    Ui::form *ui;
    Screen currentScreen;
    int currentEditID = -1;

    QSqlDatabase db;
    QSqlQuery * m_query;

    QStringList getNamesListFromTable(QString tablename);
    QString getCellFromTable(QString target, QString tablename, QString key, QString value);
    addmodel addModelForm;
    addformat addFormatForm;
    addcommand addCommandForm;
    addvariable addVariableForm;
    sqlform sqlForm;

    void createDB();
    void setWidgetInfo(QString label1, QString data1,
                       QString label2, QString data2,
                       QString label3, QString data3,
                       QString label4, QString data4,
                       QString label5, QString data5,
                       QString label6, QString data6,
                       QString label7, QString data7,
                       QString description);

    QString sProjectDir;
    QString sDB_tag;

    QString m_log_name;

    void generateCore();
    void generateModels();
    void generateFormats();
    void generateManager();
    void generateGlobalProject();
    void saveCodeToFile(const QStringList code, QFile &file);
};
#endif // FORM_H
