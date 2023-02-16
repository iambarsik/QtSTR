#ifndef ADDFORM_H
#define ADDFORM_H

#include <QDialog>
#include <QStringList>

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
    void setCurrentData(QString name, QString object, QString system, QString description);

signals:
    void signalAddModel(QString,QString,QString,QString);
    void signalUpdateModel(int,QString,QString,QString,QString);

private slots:
    void on_bCancel_clicked();

    void on_bAdd_clicked();

private:
    Ui::addform *ui;
    bool EditMode;
    int ID = -1;
};

#endif // ADDFORM_H
