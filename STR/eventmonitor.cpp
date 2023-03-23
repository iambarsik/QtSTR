#include "eventmonitor.h"
#include "ui_eventmonitor.h"
#include <QTime>

eventmonitor::eventmonitor(CoreQ *core, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::eventmonitor)
{
    ui->setupUi(this);

    this->core = core;
    this->setWindowTitle("Монитор событий");

    bLast = true;
    currentPage = 1;
    maxLines = 15;

    maxPage = core->commands.getEventCount()/maxLines + 1;
    update();
}

eventmonitor::~eventmonitor()
{
    core = nullptr;
    delete ui;
}

void eventmonitor::update()
{
    ui->listWidget->clear();
    buffer.clear();

    maxPage = core->commands.getEventCount()/maxLines + 1;

    if(bLast)   {
        currentPage = maxPage;
    }

    if(core->commands.getEventCount() <= (int)maxLines)    {
        for(int i = 0; i < core->commands.getEventCount(); i++) {
            buffer.append(convertMSecToString(core->commands.getEvent(i).time) + " " + core->commands.getCommandDescription(core->commands.getEvent(i).code));
        }
    } else {
        if(currentPage == maxPage)    {
            for(int i = (currentPage - 1)*maxLines; i < core->commands.getEventCount(); i++)    {
                buffer.append(convertMSecToString(core->commands.getEvent(i).time) + " " + core->commands.getCommandDescription(core->commands.getEvent(i).code));
            }
        } else {
            for(uint i = 0; i < maxLines; i++) {
                buffer.append(convertMSecToString(core->commands.getEvent(i).time) + " " + core->commands.getCommandDescription(core->commands.getEvent(i + (currentPage - 1)*maxLines).code));
            }
        }
    }
    for(int i = 0; i < buffer.size(); i++)    {
        ui->listWidget->insertItem(ui->listWidget->count(),buffer[i]);
    }
    ui->label->setText(QString("%1 из %2").arg(currentPage).arg(maxPage));
}

void eventmonitor::on_checkBox_stateChanged(int arg1)
{
    bLast = arg1;
    update();
}

void eventmonitor::on_pushButtonPrev_clicked()
{
    if((currentPage - 1) > 0) currentPage--;
        update();
}

void eventmonitor::on_pushButtonNext_clicked()
{
    if((currentPage + 1) <= maxPage) currentPage++;
    update();
}

QString eventmonitor::convertMSecToString(qint64 msec)
{
    QString str;
    qint64 ms = msec;
    int h, m, s;
    h = ms/3600000;
    ms = ms - h*3600000;
    m = ms/60000;
    ms = ms - m*60000;
    s = ms/1000;
    ms = ms - s*1000;
    QTime t;
    t.setHMS(h,m,s,ms);
    str = t.toString();

    return str;
}
