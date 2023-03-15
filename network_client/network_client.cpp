#include "network_client.h"

NetworkClient::NetworkClient(STRNode NodeInfo)
    : bConnected(false)
{
    socket = new QTcpSocket(this);

    Node = NodeInfo;

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket,SIGNAL(connected()),this,SLOT(slotStopReConnect()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(slotStartReConnect()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(getErrorCode(QAbstractSocket::SocketError)));

    m_timer = new QTimer;
    m_timer->setInterval(100);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(OnTimer()));

    reconnect_timer = new QTimer;
    reconnect_timer->setInterval(1000);
    connect(reconnect_timer,SIGNAL(timeout()),this,SLOT(connectToServer()));
    reconnect_timer->start();

        // try to connect to server
    //connectToServer();
}

NetworkClient::~NetworkClient()
{
    socket->disconnectFromHost();
    socket->deleteLater();
}

void NetworkClient::connectToServer()
{
    qDebug() << " trying to connect";
    socket->connectToHost(Node.host,Node.port);
    if(!socket->waitForConnected(10))  {
        qDebug() << "System :: connecting ERROR";
        return;
    }
    m_timer->start();
    bConnected = true;
    qDebug() << "Connected to Server";
    qDebug() << QString("System :: connected to server");
    sendMessage(command_type::com_init);
}

void NetworkClient::OnTimer()  {
    if(bConnected == true)  {
        sendMessage(command_type::com_ping);
    }
}

void NetworkClient::discardSocket()    {
    //socket->deleteLater();
    //socket=nullptr;
    qDebug() << "Disconnected from server!";
}

void NetworkClient::getErrorCode(QAbstractSocket::SocketError errorCode)   {
    qDebug() << "Socket Error = " << errorCode;
}


void NetworkClient::readyRead()   {
    if( socket->state() == QAbstractSocket::ConnectedState )    {
        bufferRead = socket->readAll();
        //qDebug() << bufferRead;

        if(bufferRead.length() >= 32)   {

            bool status;
            int iPackageFlag = bufferRead.mid(0,1).toHex().toInt(&status,16);
            if(iPackageFlag == 0)   {

                QByteArray header;
                header = bufferRead.mid(0,8);

                int iSize = header.mid( 1, 1).toHex().toInt(&status, 16);
                int iClientNode = header.mid( 3, 1).toHex().toInt(&status, 16);
                int iNetworkFlag = header.mid( 4, 1).toHex().toInt(&status, 16);
                int iFrameType = header.mid( 5, 1).toHex().toInt(&status, 16);

                if(iNetworkFlag == 0)   {
                    qDebug() << "Have got package from server ID: " << iClientNode;
                } else {
                    qDebug() << "Have got package from client ID: " << iClientNode;
                }
                qDebug() << "Size = " << iSize << " FrameType = " << iFrameType;
                qDebug() << socket->bytesAvailable();

                    // checking for default command
                if(iSize == 32) {
                    QByteArray frame;
                    frame = bufferRead.mid(0,32);
                    frame.remove(0,8);

                    if(frame.length() >= 24)    {                   // checking if frame is correct
                        bool status;
                        uint iCommand = frame.mid( 0,4).toHex().toInt(&status,16);

                        STR_PARAM p1 = STR_PARAM(frame.mid( 4,8));
                        STR_PARAM p2 = STR_PARAM(frame.mid( 12,8));

                        //qint64 iPar1    = frame.mid( 4,8).toHex().toInt(&status,16);
                        //qint64 iPar2    = frame.mid( 8,8).toHex().toInt(&status,16);
                        qint32 iTime    = frame.mid(20,4).toHex().toInt(&status,16);
                        bufferRead.remove(0, 32);                   // delete frame from buffer

                        qDebug() << "Input command : " << iCommand << " p1: " << p1.toInt() << " p2: " << p2.toInt() << " time: " << iTime;
                    }
                } else {
                    /*
                    // try to unpack big package
                    qDebug() << "Recieved package";
                    QByteArray package = bufferRead.mid(0,iSize);
                    package.remove(0,8);
                    //qDebug() << package;
                    emit signalPackageFromServer(package);
                    */
                }
            } else {
                // have got core package flag
                bool status;
                QByteArray package = bufferRead;
                package.remove(0,8); // delete header
                int iPackageSize = package.mid(0,4).toHex().toInt(&status,16);
                qDebug() << "Package size = :" << iPackageSize;
                package.remove(0,4); // delete 4 bytes about package size
                emit signalPackageFromServer(package);
            }
        }
    }
}

void NetworkClient::slotStopReConnect()
{
    reconnect_timer->stop();
    m_timer->start();
}

void NetworkClient::slotStartReConnect()
{
    bConnected = false;
    reconnect_timer->start();
    m_timer->stop();
}

void NetworkClient::sendMessage(command_type type)   {
    switch(type)    {
        case com_init: {
            if(socket)  {
                if(socket->isOpen())    {

                    dataSend.clear();

                    dataSend.append((char) 0x00);
                    dataSend.append((char) 13 + Node.name.size());
                    dataSend.append((char) Node.mainNode);
                    dataSend.append((char) Node.ID);
                    dataSend.append((char) 0x01);
                    dataSend.append((char) Node.frameType);
                    dataSend.append((char) 0x00);
                    dataSend.append((char) 0x00);

                    dataSend.append((char) 0xC0);
                    dataSend.append((char) 0x00);
                    dataSend.append((char) 0x00);
                    dataSend.append((char) 0x03);

                    dataSend.append(Node.name);
                    dataSend.append((char) 0x00);

                    qDebug() << "Init command is sent";

                    socket->write(dataSend);
                    socket->flush();
                    socket->waitForBytesWritten(10000);

                } else {
                    qDebug() << "Socket doesn't seem to be opened";
                }
            } else {
                qDebug() << "Not connected";
            }
        } break;
        case com_ping: {
            dataSend.clear();
            // 8 байт - заголовок кадра
            dataSend.append((char) 0x00);           // 1 байт - 0
            dataSend.append((char) 0x08);           // 2 байт - текущая длинна кадра в байтах (включая размер заголовка)
            dataSend.append((char) Node.mainNode);  // 3 байт - идентификатор получателя
            dataSend.append((char) Node.ID);        // 4 байт - идентификатор отправителя (номер узла)
            dataSend.append((char) 0x01);           // 5 байт - флаг кадра ( 0 - я сервер, 1 - я клинет )
            dataSend.append((char) Node.frameType); // 6 байт - тип кадра
            dataSend.append((char) 0x00);           // 7 байт - зарезервировано
            dataSend.append((char) 0x00);           // 8 байт - зарезервировано
            socket->write(dataSend);
            socket->flush();
            socket->waitForBytesWritten(10000);
        } break;
        case com_buffer: {
            dataSend.clear();
                // 8 байт - заголовок кадра
            dataSend.append((char) 0x00);                       // 1 байт - 0
            dataSend.append((char) 8 + bufferSend.size() + 4);  // 2 байт - текущая длинна кадра в байтах (размер заголовка + size + time)
            dataSend.append((char) Node.mainNode);              // 3 байт - идентификатор получателя
            dataSend.append((char) Node.ID);                    // 4 байт - идентификатор отправителя (номер узла)
            dataSend.append((char) 0x01);                       // 5 байт - флаг кадра ( 0 - я сервер, 1 - я клинет )
            dataSend.append((char) Node.frameType);             // 6 байт - тип кадра
            dataSend.append((char) 0x00);                       // 7 байт - зарезервировано
            dataSend.append((char) 0x00);                       // 8 байт - зарезервировано

                // 4 байта - модельное время команды в миллисекундах
            bufferSend.append((char) 0x00);
            bufferSend.append((char) 0x00);
            bufferSend.append((char) 0x00);
            bufferSend.append((char) 0x00);

            dataSend.append(bufferSend);

            qDebug() << "Length: " << dataSend.length();

            bufferSend.clear(); // очищаем буфер

            socket->write(dataSend);
            socket->flush();
            socket->waitForBytesWritten(10000);
        } break;
        case command_type::com_package: {

        } break;
        case command_type::com_na:  {

        } break;
    }
}

void NetworkClient::addCommand(command_t command)  {

    bufferSend.clear();
    bufferSend.append((command.code >> 24) & 0xFF);
    bufferSend.append((command.code >> 16) & 0xFF);
    bufferSend.append((command.code >> 8)  & 0xFF);
    bufferSend.append((command.code) & 0xFF);
    bufferSend.append(command.par1.data);
    bufferSend.append(command.par2.data);

    qDebug() << QString("Command is sent: %1 %2 %3").arg(command.code).arg(command.par1.toInt()).arg(command.par2.toInt());
    sendMessage(command_type::com_buffer);
}


