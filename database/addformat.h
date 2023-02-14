#ifndef ADDFORMAT_H
#define ADDFORMAT_H

#include <QDialog>

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

signals:
    void signalAddFormat(QString,QString,QString,QString,QString);
    void signalUpdateFormat(int,QString,QString,QString,QString,QString);

private slots:
    void on_bCancel_clicked();

    void on_bAdd_clicked();

private:
    Ui::addformat *ui;
    bool EditMode;
    int ID = -1;
};

#endif // ADDFORMAT_H
