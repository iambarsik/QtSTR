#ifndef ADDVARIABLE_H
#define ADDVARIABLE_H

#include <QDialog>

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
    void setCurrentData(QString name, QString model, QString type, uint size, QString description);

signals:
    void signalAddVariable(QString,QString,QString,uint,QString);
    void signalUpdateVariable(int,QString,QString,QString,uint,QString);

private slots:
    void on_bAdd_clicked();

    void on_bCancel_clicked();

private:
    Ui::addvariable *ui;
    bool EditMode;
    int ID = -1;
};

#endif // ADDVARIABLE_H
