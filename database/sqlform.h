#ifndef SQLFORM_H
#define SQLFORM_H

#include <QDialog>

namespace Ui {
class sqlform;
}

class sqlform : public QDialog
{
    Q_OBJECT

public:
    explicit sqlform(QWidget *parent = nullptr);
    ~sqlform();

signals:
    void signalSQLrequest(QString);

private slots:
    void on_bCancel_clicked();

    void on_bExecute_clicked();

private:
    Ui::sqlform *ui;
};

#endif // SQLFORM_H
