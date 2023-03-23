#ifndef EVENTMONITOR_H
#define EVENTMONITOR_H

#include <QDialog>
#include <QList>

#include "../include/common.h"
#include "../core/core.h"

namespace Ui {
class eventmonitor;
}

class eventmonitor : public QDialog
{
    Q_OBJECT

public:
    explicit eventmonitor(CoreQ *core, QWidget *parent = nullptr);
    ~eventmonitor();

public slots:
    void update();

private slots:
    void on_checkBox_stateChanged(int arg1);

    void on_pushButtonPrev_clicked();

    void on_pushButtonNext_clicked();

private:
    Ui::eventmonitor *ui;

    CoreQ *core;

    bool bLast;
    uint currentPage;
    uint maxPage;
    uint maxLines;  // max line on one page

    QList<QString> buffer;
    QString convertMSecToString(qint64 msec);


};

#endif // EVENTMONITOR_H
