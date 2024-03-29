#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include <QDialog>
#include "data_struct.h"

namespace Ui {
class addcommand;
}

class addcommand : public QDialog
{
    Q_OBJECT

public:
    explicit addcommand(QWidget *parent = nullptr);
    ~addcommand();

    void setData(const QStringList Models,const QStringList Types, bool EditMode = false, int id = -1);
    void setCurrentData(command_struct com);

signals:
    void signalAddCommand(command_struct);
    void signalUpdateCommand(command_struct);

private slots:
    void on_bCancel_clicked();

    void on_bAdd_clicked();

private:
    Ui::addcommand *ui;
    bool EditMode;
    int ID = -1;
};

#endif // ADDCOMMAND_H
