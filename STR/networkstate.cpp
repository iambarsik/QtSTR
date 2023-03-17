#include "networkstate.h"
#include "ui_networkstate.h"

#include <QDebug>
#include <QIcon>

networkstate::networkstate(QList<STRNodeInfo> *nodes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::networkstate)
{
    ui->setupUi(this);
    this->setWindowTitle("Состояние сети");

    Nodes = nodes;
    update();

    this->setWindowIcon(QIcon(":/images/ico.ico"));
}


networkstate::~networkstate()
{
    delete ui;
}

void networkstate::update()
{
    for (int i = 0; i < pictureList.size(); i++) {
        delete pictureList[i];
    }
    for (int i = 0; i < textList.size(); i++) {
        delete pictureList[i];
    }

    int row = 0;
    int col = 0;
    int maxCol = 7;
    for(int i = 0; i < Nodes->size(); i++)   {
        if(col >= maxCol)    {
            row++;
            col = 0;
        }

        QLabel *label = new QLabel(this);
        label->setGeometry(25 + col*75, 15 + row*65, 50, 50);
        label->setScaledContents(true);
        QPixmap p(QString(":/images/%1.png").arg(Nodes->at(i).state));
        label->setPixmap(p);
        label->show();

        QLabel *text = new QLabel(this);
        text->setText(Nodes->at(i).name);
        text->setAlignment(Qt::AlignCenter);
        text->setGeometry(5 + col*75, row*65, 90, 15);
        text->show();

        col++;

        qDebug() << "name:" << Nodes->at(i).name;
    }

    row++;
    if(row > 1) {
        this->setMinimumSize(QSize(25 + maxCol*(75),row*65 + 5));
        this->setMaximumSize(QSize(25 + maxCol*(75) + 1,row*65 + 5 + 1));
    } else {
        this->setMinimumSize(QSize(25 + col*(75),row*65 + 5));
        this->setMaximumSize(QSize(25 + col*(75) + 1,row*65 + 5 + 1));
    }
}
