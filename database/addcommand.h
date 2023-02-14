#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include <QDialog>

namespace Ui {
class addcommand;
}

class addcommand : public QDialog
{
    Q_OBJECT

public:
    explicit addcommand(QWidget *parent = nullptr);
    ~addcommand();

    void setData(const QStringList Models, bool EditMode = false, int id = -1);

signals:
    void signalAddCommand(QString,QString,QString);
    void signalUpdateCommand(int,QString,QString,QString);

private slots:
    void on_bCancel_clicked();

    void on_bAdd_clicked();

private:
    Ui::addcommand *ui;
    bool EditMode;
    int ID = -1;
};

#endif // ADDCOMMAND_H
