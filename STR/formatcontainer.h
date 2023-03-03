#ifndef FORMATCONTAINER_H
#define FORMATCONTAINER_H

#include <QDialog>
#include <manager.h>
#include "../include/common.h"

namespace Ui {
class formatcontainer;
}

struct FormatItem   {
    QString title;
    STRformat_enum name;
};

struct SystemItem   {
    QString title;
    QList<FormatItem> formList;
};

struct ObjectItem   {
    QString title;
    QList<SystemItem> sysList;
};

class formatcontainer : public QDialog
{
    Q_OBJECT

public:
    formatcontainer(QList<STRFormat<STRformat_enum>> formatList, QWidget *parent = nullptr);
    ~formatcontainer();

signals:
    void signalOpenFormat(STRformat_enum);

private slots:
    void on_cbObject_currentIndexChanged(int index);
    void on_listSystem_currentRowChanged(int currentRow);

    void on_bCancel_clicked();

    void on_bOpen_clicked();

private:
    Ui::formatcontainer *ui;
    QList<STRFormat<STRformat_enum>> formatList;
    QList<ObjectItem> FormContainer;

    void initData();
    void updateSystems();
    void updateFormats();
};

#endif // FORMATCONTAINER_H
