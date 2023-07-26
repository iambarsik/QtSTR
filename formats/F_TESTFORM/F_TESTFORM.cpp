#include "F_TESTFORM.h"
#include "ui_F_TESTFORM.h"

F_TESTFORM::F_TESTFORM(CoreQ * core, QWidget *parent)
    : FormatQ(core, FORMAT_ID, "F_TESTFORM", "НЭМ", "СУБА", parent)
    , ui(new Ui::F_TESTFORM)
{
    ui->setupUi(this);
}

F_TESTFORM::~F_TESTFORM()
{
    delete ui;
}

void F_TESTFORM::update()
{

    repaint();
}

void F_TESTFORM::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setBrush(QBrush(Qt::black));
    p.drawRect(0,0,this->width(),this->height());

    p.setPen(QPen(Qt::green));
    p.setFont(QFont("Arial",12));
    p.drawText(10,14*1,QString("%1").arg(core->get_test_double1()));
    p.drawText(10,14*2,QString("%1").arg(core->get_test_double2()));
    p.drawText(10,14*3,QString("%1").arg(core->get_test_double3()));

    /*
    sendCommandToModel(core->commands.test_command7,
                       STR_PARAM(0.0),
                       STR_PARAM(0.0), 0);
    */
}
