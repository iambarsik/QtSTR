#ifndef ADDFORM_H
#define ADDFORM_H

#include <QDialog>
#include <QStringList>
#include "data_struct.h"

namespace Ui {
class addform;
}

class addmodel : public QDialog
{
    Q_OBJECT

public:
    explicit addmodel(QWidget *parent = nullptr);
    ~addmodel();

    void setData(const QStringList Objects, const QStringList Systems, bool EditMode = false, int id = -1);
    void setCurrentData(model_struct mod);

signals:
    void signalAddModel(model_struct);
    void signalUpdateModel(model_struct);

private slots:
    void on_bCancel_clicked();

    void on_bAdd_clicked();

private:
    Ui::addform *ui;
    bool EditMode;
    int ID = -1;
};

#endif // ADDFORM_H
