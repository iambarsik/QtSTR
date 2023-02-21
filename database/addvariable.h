#ifndef ADDVARIABLE_H
#define ADDVARIABLE_H

#include <QDialog>
#include "data_struct.h"

namespace Ui {
class addvariable;
}

class addvariable : public QDialog
{
    Q_OBJECT

public:
    explicit addvariable(QWidget *parent = nullptr);
    ~addvariable();

    void setData(const QStringList Models,const QStringList Types, bool EditMode = false, int id = -1);
    void setCurrentData(variable_struct var);

signals:
    void signalAddVariable(variable_struct);
    void signalUpdateVariable(variable_struct);

private slots:
    void on_bAdd_clicked();

    void on_bCancel_clicked();

private:
    Ui::addvariable *ui;
    bool EditMode;
    int ID = -1;
};

#endif // ADDVARIABLE_H
