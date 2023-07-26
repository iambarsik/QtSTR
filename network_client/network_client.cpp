#include "network_client.h"

NetworkClient::NetworkClient(STRNode NodeInfo, bool NA_mode)
    : bConnected(false)
{
    socket = new QTcpSocket(this);

    Node = NodeInfo;
    bNA_MODE = NA_mode;

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
    bConnected = true;
    reconnect_timer->stop();
    m_timer->start();
    qDebug() << "Connected to Server";
    qDebug() << QString("System :: connected to server");
    sendMessage(network_command_type::com_init);
    sendMessage(network_command_type::com_ping);
    emit clientConnected();
}

void NetworkClient::OnTimer()  {
    if(bConnected == true)  {
        sendMessage(network_command_type::com_ping);
    }
}

void NetworkClient::discardSocket()    {
    qDebug() << "Disconnected from server!";
}

void NetworkClient::getErrorCode(QAbstractSocket::SocketError errorCode)   {
    qDebug() << "Socket Error = " << errorCode;
}


void NetworkClient::readyRead()   {
    if( socket->state() == QAbstractSocket::ConnectedState )    {

        bufferRead.clear();
        bufferRead = socket->readAll();

        //qDebug() << "Length: " << bufferRead.length();
        //qDebug() << bufferRead;

            // use default STR mode for network
        /*
         * 8 bytes header
         * 4 bytes command
         * 8 bytes par 1 (for double supporting)
         * 8 bytes par 2 (for double supporting)
         * 4 bytes model time sending
         *
         * total: 32 bytes for default STR command
         *
         * */
        if(bNA_MODE == false)    {
            if(bufferRead.length() >= 32)   {

                bool status;
                int iPackageFlag = bufferRead.mid(0,1).toHex().toInt(&status,16);

                if(iPackageFlag == 0)   {

                    for(int byte = 0; byte < bufferRead.length(); byte+= 32)    {

                        qDebug() << bufferRead.length();

                        QByteArray header;
                        header = bufferRead.mid(byte,8);

                        int iSize = header.mid( 1, 1).toHex().toInt(&status, 16);
                        int iClientNode = header.mid( 3, 1).toHex().toInt(&status, 16);
                        int iNetworkFlag = header.mid( 4, 1).toHex().toInt(&status, 16);
                        int iFrameType = header.mid( 5, 1).toHex().toInt(&status, 16);

                        /*
                        if(iNetworkFlag == 0)   {
                            qDebug() << "Have got message from server ID: " << iClientNode;
                        } else {
                            qDebug() << "Have got message from client ID: " << iClientNode;
                        }
                        */
                        qDebug() << "Size = " << iSize << " FrameType = " << iFrameType;

                            // checking for default command
                        if(iSize == 32) {
                            QByteArray frame;
                            frame = bufferRead.mid(byte,32);
                            frame.remove(0,8);

                            if(frame.length() >= 24)    {                   // checking if frame is correct
                                bool status;
                                uint iCommand = frame.mid( 0,4).toHex().toUInt(&status,16);

                                STR_PARAM p1 = STR_PARAM(frame.mid( 4,8));
                                STR_PARAM p2 = STR_PARAM(frame.mid( 12,8));

                                qint32 iTime    = frame.mid(20,4).toHex().toInt(&status,16);

                                qDebug() << "Input command from STR : " << iCommand << " p1: " << p1.toInt() << " p2: " << p2.toInt() << " time: " << iTime;
                                emit signalCommandFromServer({iCommand, p1, p2, iTime});
                            }

                            bufferRead.remove(0,32);
                        }
                    }



                } else {
                    // have got core package flag
                    bool status;
                    QByteArray package = bufferRead;
                    package.remove(0,8); // delete header
                    int iPackageSize = package.mid(0,4).toHex().toInt(&status,16);
                    //qDebug() << "Package size = " << iPackageSize;
                    package.remove(0,4); // delete 4 bytes about package size
                    emit signalPackageFromServer(package);
                }
            }

        } else {

            // working with NA abonent
            /*
            * 8 bytes header
            * 4 bytes command
            * 4 bytes par 1 (double not suppots)
            * 4 bytes par 2 (double not suppots)
            * 4 bytes model time sending
            *
            * total: 24 bytes for default NA or TRIO command
            *
            * */
            for(int byte = 0; byte < bufferRead.length(); byte+= 24)    {
                bool status;
                QByteArray header;
                header = bufferRead.mid(byte,8);
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

                    // checking for default command
                if(iSize == 24) {
                    QByteArray frame;
                    frame = bufferRead.mid(byte,24);
                    frame.remove(0,8);
                    if(frame.length() >= 16)    {                   // checking if frame is correct
                        bool status;
                        uint iCommand = frame.mid( 0,4).toHex().toInt(&status,16);

                        qint32 p1 = frame.mid( 4,4).toHex().toInt(&status, 16);
                        qint32 p2 = frame.mid( 8,4).toHex().toInt(&status, 16);

                        qint32 iTime    = frame.mid(12,4).toHex().toInt(&status,16);
                        bufferRead.remove(0, 24);                   // delete frame from buffer

                        emit signalCommandFromNA({iCommand, p1, p2, iTime});

                        qDebug() << "Input command from NA : " << iCommand << " p1: " << p1 << " p2: " << p2 << " time: " << iTime;
                    }
                } else {
                    // have got core package flag
                    QByteArray package = bufferRead;
                    package.remove(0,8); // delete header
                    qDebug() << "Input package from NA. Size = " << iSize;
                    qint32 iCommand = package.mid( 0,4).toHex().toInt(&status,16);
                    package.remove(0,4); // delete command

                    emit signalPackageFromNA({iCommand,package});
                }
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
    emit clientDisconnected();
    reconnect_timer->start();
    m_timer->stop();
}

void NetworkClient::sendMessage(network_command_type type)   {
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
        case network_command_type::com_package: {

        } break;
        case network_command_type::com_na:  {
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
    sendMessage(network_command_type::com_buffer);
}

void NetworkClient::addCommandForNA(qint32 command, qint32 par1, qint32 par2)
{
    bufferSend.clear();
    bufferSend.append((command >> 24) & 0xFF);
    bufferSend.append((command >> 16) & 0xFF);
    bufferSend.append((command >> 8)  & 0xFF);
    bufferSend.append((command) & 0xFF);
    bufferSend.append((par1 >> 24) & 0xFF);
    bufferSend.append((par1 >> 16) & 0xFF);
    bufferSend.append((par1 >> 8)  & 0xFF);
    bufferSend.append((par1) & 0xFF);
    bufferSend.append((par2 >> 24) & 0xFF);
    bufferSend.append((par2 >> 16) & 0xFF);
    bufferSend.append((par2 >> 8)  & 0xFF);
    bufferSend.append((par2) & 0xFF);

    qDebug() << QString("Command for NA is sent: %1 %2 %3").arg(command).arg(par1).arg(par2);
    sendMessage(network_command_type::com_na);
}


