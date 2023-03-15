#include "str.h"
#include "ui_str.h"

#include <QScreen>
#include <QSharedPointer>

#include <QTextCodec>

STR::STR(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::STR)
    , bServerNode(true)
    , multiScreen(false)
{
    ui->setupUi(this);

    CurrentNode.name      = "STR application";
    CurrentNode.host      = "127.0.0.1";
    CurrentNode.port      = 2001;
    CurrentNode.frameType = 0;
    CurrentNode.mainNode  = 2;
    CurrentNode.ID        = 2;
    CurrentNode.isServer  = bServerNode;

    QString SettingsName = qApp->applicationDirPath() + "/config.ini";
    QSettings settings(SettingsName, QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
    if(QFile::exists(SettingsName))    {
        settings.beginGroup("MAIN_SETTINGS");

        CurrentNode.isServer  = (settings.value("server", false)).toBool();
        CurrentNode.host      = (settings.value("host", "127.0.0.1")).toString();
        CurrentNode.port      = (settings.value("port", 2001)).toInt();
        CurrentNode.name      = (settings.value("name", "Клиент приложения")).toString();
        CurrentNode.frameType = (settings.value("frameType", 0)).toInt();
        CurrentNode.mainNode  = (settings.value("mainNode", 2)).toInt();
        CurrentNode.ID        = (settings.value("nodeID", 0)).toInt();
        multiScreen           = (settings.value("multiScreen", false)).toBool();

        settings.endGroup();
        qDebug() << "config loaded";
    } else {
        settings.beginGroup("MAIN_SETTINGS");

        settings.setValue("server",true);
        settings.setValue("host","127.0.0.1");
        settings.setValue("port",2001);
        settings.setValue("name","Сервер приложения");
        settings.setValue("frameType",0);
        settings.setValue("mainNode",2);
        settings.setValue("nodeID",2);
        settings.setValue("multiScreen",false);

        settings.endGroup();
        qDebug() << "config created";
    }

    bServerNode = CurrentNode.isServer;

        // get screen data
    QScreen* screen = QApplication::screens().at(0);
    QSize size = screen->availableSize();
    this->setGeometry(0,0,size.width(),size.height());
    ui->mdiArea->setGeometry(0,40,size.width(),size.height() - 100);

        // stretch window for all screens
    if(multiScreen) {
        if(QApplication::screens().size() > 1)  {
                // find minimum screen height
            int iHeight = 99999;
            for(int i = 0; i < QApplication::screens().size(); i++) {
                if(QApplication::screens().at(i)->geometry().height() < iHeight)
                    iHeight = QApplication::screens().at(i)->geometry().height();
            }
                // calculate biggest width
            int iWidth = 0;
            for(int i = 0; i < QApplication::screens().size(); i++) {
                iWidth += QApplication::screens().at(i)->geometry().width();
            }
            this->setGeometry(0,0,iWidth,iHeight);
            ui->mdiArea->setGeometry(0,50,this->width(),this->height() - 100);
        }
    }

    core = new CoreQ;
    connect(this,&STR::start,core,&CoreQ::startCore);
    connect(this,&STR::stop,core,&CoreQ::stopCore);

    object_manager = new ManagerQ(core);

    if(bServerNode) {
        STR_server = new NetworkServer(CurrentNode);
        connect(STR_server,SIGNAL(signalClientsAreConnected(int)),
                this,SLOT(setClientInformation(int)));
        connect(STR_server,SIGNAL(signalCommandFromClient(command_t)),
                this,SLOT(slotReadCommand(command_t)));
        for(int i = 0; i < object_manager->modelList.size(); i++) {
            ConnectModel(object_manager->modelList[i]);
        }
    } else {
        STR_client = new NetworkClient(CurrentNode);
        connect(STR_client, SIGNAL(signalPackageFromServer(QByteArray)),
                core, SLOT(setCoreFromPackage(QByteArray)));
    }

    this->setWindowTitle(CurrentNode.name);

    STR_timer = new QTimer;
    STR_timer->setInterval(100);
    connect(STR_timer,SIGNAL(timeout()),
            this, SLOT(slotTimer()));

    STR_timer->start();

}

STR::~STR()
{
    emit stop();
    STR_timer->stop();
    STR_timer->disconnect();

    delete core;

    if(bServerNode) {
        delete STR_server;
    } else {
        delete STR_client;
    }
    delete STR_timer;
    delete ui;
}

void STR::slotTimer()   {
    if(bServerNode == true)   {
        STR_server->addPackage(core->getPackage());
    } else {

        if(STR_client->isConnected())   {
            this->setWindowTitle(QString("%1. Подключено к серверу").arg(CurrentNode.name));
        } else {
            this->setWindowTitle(QString("%1. Нет соединения с сервером").arg(CurrentNode.name));
        }
    }

    if(core->isStartCore()) {
        ui->label->setText("Состояние: ПУСК");
    } else {
        ui->label->setText("Состояние: СТОП");
    }
}

void STR::setClientInformation(int value)
{
    this->setWindowTitle(QString("%1. Подключено %2 клиентов").arg(CurrentNode.name).arg(value));
}

void STR::slotOpenFormat(STRformat_enum name)
{
    FormatQ *f = object_manager->openFormat(name);
    ConnectFormat(f);
    ui->mdiArea->addSubWindow(f);
    f->show();
}

void STR::on_pushButtonSet_clicked()
{
    if(bServerNode) {
        emit stop();
        emit set();
    } else {
        command_t com;
        com.code = 0x80000001;
        com.par1 = STR_PARAM((qint64)0);
        com.par2 = STR_PARAM((qint64)0);
        com.time = 0;
        STR_client->addCommand(com);
    }
}

void STR::on_pushButtonStart_clicked()
{
    if(bServerNode) {
        emit start();
    } else {
        command_t com;
        com.code = 0x80000002;
        com.par1 = STR_PARAM((qint64)0);
        com.par2 = STR_PARAM((qint64)0);
        com.time = 0;
        STR_client->addCommand(com);
    }
}

void STR::on_pushButtonStop_clicked()
{
    if(bServerNode) {
        emit stop();
    } else {
        command_t com;
        com.code = 0x80000003;
        com.par1 = STR_PARAM((qint64)0);
        com.par2 = STR_PARAM((qint64)0);
        com.time = 0;
        STR_client->addCommand(com);
    }
}

void STR::on_action_triggered()
{
    exit(0);
}

void STR::on_pushButtonContainer_clicked()
{
    formatcontainer *f = new formatcontainer(object_manager->getFormatList());
    connect(f, SIGNAL(signalOpenFormat(STRformat_enum)),this, SLOT(slotOpenFormat(STRformat_enum)));
    ui->mdiArea->addSubWindow(f);
    f->show();
}
