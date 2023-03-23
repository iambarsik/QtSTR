#include "str.h"
#include "ui_str.h"

#include <QScreen>
#include <QSharedPointer>
#include <QTextCodec>
#include <QMessageBox>

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

    if(!QFile::exists(SettingsName))    {
        QStringList ini;
        ini.append("[MAIN_SETTINGS]");
        ini.append("server=true");
        ini.append("host=127.0.0.1");
        ini.append("port=2001");
        ini.append("name=Сервер приложения");
        ini.append("frameType=0");
        ini.append("mainNode=2");
        ini.append("nodeID=2");
        ini.append("multiScreen=false");
        ini.append("");
        ini.append("#[NODE-9]");
        ini.append("#nodeID=9");
        ini.append("#host=127.0.0.1");
        ini.append("#port=2009");
        ini.append("#title=NODE NAME");
        ini.append("#frameType=9");
        ini.append("#modelName=M_TEST");
        ini.append("");

        QFile file(SettingsName);
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out.setCodec(QTextCodec::codecForName("UTF-8"));
        for(int i = 0; i < ini.size(); i++)   {
            out << ini[i] << "\n";
        }
        file.close();

        qDebug() << "STR :: config created";

    } else {

            // get nodes list
        QStringList groupNames;

        QFile file(SettingsName);
        if(!file.open(QFile::ReadWrite))    {
            qDebug() << "STR :: config file open error";
            QMessageBox::warning(this, "Ошибка!", "Невозможно открыть файл конфигурации!", QMessageBox::Yes);
            exit(EXIT_FAILURE);
        }

        QTextStream out(&file);
        QString line;
        while (out.readLineInto(&line)) {
            if(line.startsWith("[NODE-"))   {
                QString s = line;
                s.remove('[');
                s.remove(']');
                groupNames.append(s);
            }
            line.clear();
        }
        file.close();

        QSettings settings(SettingsName, QSettings::IniFormat);
        settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

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

        for(int i = 0; i < groupNames.size(); i++)  {
            STRNode node;

            settings.beginGroup(groupNames[i]);
            node.ID        = (settings.value("nodeID", 0)).toInt();
            node.host      = (settings.value("host", "127.0.0.1")).toString();
            node.port      = (settings.value("port", 2001)).toInt();
            node.name      = (settings.value("title", "unknown")).toString();
            node.frameType = (settings.value("frameType", 0)).toInt();
            node.modelName = (settings.value("modelName", "M_TEST")).toString();
            settings.endGroup();

            NA_Nodes.push_back(node);
        }
        qDebug() << "STR :: config loaded";
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


        /*
         *  need to fix non complete synchronizing
         *
         *
        connect(STR_server,SIGNAL(signalNewClientConnected(QTcpSocket*)),
                this, SLOT(slotNewClientConnected(QTcpSocket*)));
        */

        for(int i = 0; i < object_manager->modelList.size(); i++) {
            ConnectModel(object_manager->modelList[i]);
        }


            // init and connect all NA clients that are read from config file
        for(int i = 0; i < NA_Nodes.size(); i++)    {
            NetworkClient * client = new NetworkClient(NA_Nodes[i],true);            

            connect(client,SIGNAL(clientConnected()),this,SLOT(updateNetworkState()));
            connect(client,SIGNAL(clientDisconnected()),this,SLOT(updateNetworkState()));

                // search and connect model owner for each NA node
            for(int m = 0; m < object_manager->modelList.size(); m++)   {
                if(object_manager->modelList[m]->getModelName() == NA_Nodes[i].modelName)   {
                    connect(client,SIGNAL(signalCommandFromNA(command_na)),
                            object_manager->modelList[m],SLOT(ReadCommandFromNA(command_na)));
                    connect(client,SIGNAL(signalPackageFromNA(package_na)),
                            object_manager->modelList[m],SLOT(ReadPackageFromNA(package_na)));
                    break;
                }
            }

            NA_client.push_back(client);
        }

        updateNetworkState();

    } else {
        STR_client = new NetworkClient(CurrentNode);
        connect(STR_client, SIGNAL(signalPackageFromServer(QByteArray)),
                core, SLOT(setCoreFromPackage(QByteArray)));

        connect(STR_client,SIGNAL(signalCommandFromServer(command_t)),
                this,SLOT(slotReadCommand(command_t)));
    }

    this->setWindowTitle(CurrentNode.name);

    STR_timer = new QTimer;
    STR_timer->setInterval(100);
    connect(STR_timer,SIGNAL(timeout()),
            this, SLOT(slotTimer()));

    STR_timer->start();

    this->setWindowIcon(QIcon(":/images/ico.ico"));
}

STR::~STR()
{
    emit stop();
    STR_timer->stop();
    STR_timer->disconnect();

    delete core;
    delete object_manager;

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

