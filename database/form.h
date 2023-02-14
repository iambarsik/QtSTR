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

#include "addmodel.h"
#include "addformat.h"
#include "addcommand.h"
#include "addvariable.h"

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

    void slotAddModel(QString name, QString object, QString system, QString description);
    void slotUpdateModel(int,QString name, QString object, QString system, QString description);
    void slotAddFormat(QString name, QString title, QString object, QString system, QString description);
    void slotUpdateFormat(int id, QString name, QString title, QString object, QString system, QString description);
    void slotAddCommand(QString name, QString model, QString description);
    void slotUpdateCommand(int id, QString name, QString model, QString description);
    void slotAddVariable(QString name, QString model, QString type, uint size, QString description);
    void slotUpdateVariable(int id, QString name, QString model, QString type, uint size, QString description);

    void on_bAdd_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_bRemove_clicked();


    void on_bEdit_clicked();

private:
    Ui::form *ui;
    Screen currentScreen;
    int currentEditID = -1;

    QSqlDatabase db;
    QSqlQuery * m_query;
    QSqlTableModel * m_model;

    QStringList getNamesListFromTable(QString tablename);
    addmodel addModelForm;
    addformat addFormatForm;
    addcommand addCommandForm;
    addvariable addVariableForm;

    void setWidgetInfo(QString label1, QString data1,
                       QString label2, QString data2,
                       QString label3, QString data3,
                       QString label4, QString data4,
                       QString label5, QString data5,
                       QString label6, QString data6,
                       QString description);
};
#endif // FORM_H
