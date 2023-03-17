#ifndef NETWORKSTATE_H
#define NETWORKSTATE_H

#include <QDialog>
#include <QLabel>

#include "../include/common.h"

namespace Ui {
class networkstate;
}

class networkstate : public QDialog
{
    Q_OBJECT

public:
    explicit networkstate(QList<STRNodeInfo> *nodes, QWidget *parent = nullptr);
    ~networkstate();

public slots:
    void update();

private:
    Ui::networkstate *ui;
    QList<STRNodeInfo> *Nodes;
    QList<QLabel *> pictureList;
    QList<QLabel *> textList;
};

#endif // NETWORKSTATE_H
