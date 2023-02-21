#ifndef ADDFORMAT_H
#define ADDFORMAT_H

#include <QDialog>
#include "data_struct.h"

namespace Ui {
class addformat;
}

class addformat : public QDialog
{
    Q_OBJECT

public:
    explicit addformat(QWidget *parent = nullptr);
    ~addformat();

    void setData(const QStringList Objects, const QStringList Systems, bool EditMode = false, int id = -1);
    void setCurrentData(format_struct form);

signals:
    void signalAddFormat(format_struct);
    void signalUpdateFormat(format_struct);

private slots:
    void on_bCancel_clicked();

    void on_bAdd_clicked();

private:
    Ui::addformat *ui;
    bool EditMode;
    int ID = -1;
};

#endif // ADDFORMAT_H
