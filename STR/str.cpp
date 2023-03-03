#include "str.h"
#include "ui_str.h"

#include <QScreen>
#include <QSharedPointer>

STR::STR(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::STR)
    , bServerNode(true)
    , sHostName("127.0.0.1")
    , iHostPort(8080)
    , multiScreen(false)
{
    ui->setupUi(this);

    QString SettingsName = qApp->applicationDirPath() + "/config.ini";
    QSettings settings(SettingsName, QSettings::IniFormat);
    if(QFile::exists(SettingsName))    {
        settings.beginGroup("MAIN_SETTINGS");
        bServerNode   = (settings.value("server", false)).toBool();
        sHostName     = (settings.value("host", "127.0.0.1")).toString();
        iHostPort     = (settings.value("port", 8080)).toInt();
        multiScreen   = (settings.value("multiScreen", false)).toBool();
        settings.endGroup();
        qDebug() << "config loaded";
    } else {
        settings.beginGroup("MAIN_SETTINGS");
        settings.setValue("server",false);
        settings.setValue("host","127.0.0.1");
        settings.setValue("port",8080);
        settings.setValue("multiScreen",false);
        settings.endGroup();
        qDebug() << "config created";
    }

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
    //initModels();

    if(bServerNode) {
        STR_server = new NetworkServer(iHostPort);
        connect(STR_server,SIGNAL(signalClientsAreConnected(int)),
                this,SLOT(setClientInformation(int)));
        connect(STR_server,SIGNAL(signalCommandFromClient(command_t)),
                this,SLOT(slotReadCommand(command_t)));
        //for(int i = 0; i < modelList.size(); i++) {
        //    ConnectModel(modelList[i]);
        //}
        for(int i = 0; i < object_manager->modelList.size(); i++) {
            ConnectModel(object_manager->modelList[i]);
        }

        this->setWindowTitle("STR is runned as SERVER");
    } else {
        STR_client = new NetworkClient(sHostName, iHostPort);
        connect(STR_client, SIGNAL(signalPackageFromServer(QByteArray)),
                core, SLOT(setCoreFromPackage(QByteArray)));
        this->setWindowTitle("STR is runned as CLIENT");
    }

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
    //delete model_test;

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
/*
        if(STR_client->isConnected() == false)  {
            STR_client->connectToSTRServer();
        }
*/
    }

    if(core->isStartCore()) {
        ui->label->setText("Состояние: ПУСК");
    } else {
        ui->label->setText("Состояние: СТОП");
    }
}

void STR::setClientInformation(int value)
{
    this->setWindowTitle(QString("STR is runned as SERVER. %1 clients are connected").arg(value));
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

void STR::on_pushButtonTest_clicked()
{
    formatcontainer *f = new formatcontainer(object_manager->getFormatList());
    connect(f, SIGNAL(signalOpenFormat(STRformat_enum)),this, SLOT(slotOpenFormat(STRformat_enum)));
    ui->mdiArea->addSubWindow(f);
    f->show();
}

void STR::on_action_triggered()
{
    exit(0);
}
