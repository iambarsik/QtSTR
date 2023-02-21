#include "network_client.h"

NetworkClient::NetworkClient(QString Host, qint32 Port)
    : bConnected(false)
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, &QTcpSocket::disconnected, this, &NetworkClient::discardSocket);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(getErrorCode(QAbstractSocket::SocketError)));

    m_timer = new QTimer;
    m_timer->setInterval(100);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(OnTimer()));

        // try to connect to server
    try {
        socket->connectToHost(Host,Port);

        if(socket->waitForConnected())  {
            qDebug() << "Connected to Server";
            qDebug() << QString("System :: connected to server");
            bConnected = true;
            sendMessage(command_type::com_init);
        } else {
            qDebug() << "System :: connecting ERROR";
            exit(EXIT_FAILURE);
        }

        m_timer->start();
        //bInited = false;
        sendMessage(command_type::com_init);
    } catch (QException *e) {
        qDebug() << e->what();
    }

    bInit = false;

}

NetworkClient::~NetworkClient()
{
    socket->disconnectFromHost();
    socket->deleteLater();
}

void NetworkClient::OnTimer()  {
    if(bConnected == true)  {
        sendMessage(command_type::com_ping);
    }
}

void NetworkClient::discardSocket()    {
    socket->deleteLater();
    socket=nullptr;
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
                int iMainNode = header.mid( 2, 1).toHex().toInt(&status, 16);
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



void NetworkClient::sendMessage(command_type type)   {

    switch(type)    {
        case com_init:
            if(socket)  {
                if(socket->isOpen())    {
                    //if(bInited == false) {
                        int MainNode  = 2;
                        int NodeID    = 1;
                        int FrameType = 0x15;
                        QString name  = "127.0.0.1";

                        dataSend.clear();

                        dataSend.append((char) 0x00);
                        dataSend.append((char) 13 + name.size());
                        dataSend.append((char) MainNode);
                        dataSend.append((char) NodeID);
                        dataSend.append((char) 0x01);
                        dataSend.append((char) FrameType);
                        dataSend.append((char) 0x00);
                        dataSend.append((char) 0x00);

                        dataSend.append((char) 0xC0);
                        dataSend.append((char) 0x00);
                        dataSend.append((char) 0x00);
                        dataSend.append((char) 0x03);

                        dataSend.append(name);
                        dataSend.append((char) 0x00);

                        //bInited = true;

                        qDebug() << "Init command is sent";

                        socket->write(dataSend);
                        socket->flush();
                        socket->waitForBytesWritten(10000);
                    //}
                } else {
                    qDebug() << "Socket doesn't seem to be opened";
                }
            } else {
                qDebug() << "Not connected";
            }
        break;
        case com_ping:
            dataSend.clear();
            // 8 байт - заголовок кадра
            dataSend.append((char) 0x00);           // 1 байт - 0
            dataSend.append((char) 0x08);           // 2 байт - текущая длинна кадра в байтах (включая размер заголовка)
            dataSend.append((char) 0x02);           // 3 байт - идентификатор получателя
            dataSend.append((char) 0x01);           // 4 байт - идентификатор отправителя (номер узла)
            dataSend.append((char) 0x01);           // 5 байт - флаг кадра ( 0 - я сервер, 1 - я клинет )
            dataSend.append((char) 0x00);           // 6 байт - тип кадра 0
            dataSend.append((char) 0x00);           // 7 байт - зарезервировано
            dataSend.append((char) 0x00);           // 8 байт - зарезервировано
            socket->write(dataSend);
            socket->flush();
            socket->waitForBytesWritten(10000);
        break;
        case com_buffer:
            //if(bufferSend.size() == 12){
                dataSend.clear();
                    // 8 байт - заголовок кадра
                dataSend.append((char) 0x00);                       // 1 байт - 0
                dataSend.append((char) 8 + bufferSend.size() + 4);  // 2 байт - текущая длинна кадра в байтах (размер заголовка + size + time)
                dataSend.append((char) 0x02);                       // 3 байт - идентификатор получателя
                dataSend.append((char) 0x01);                       // 4 байт - идентификатор отправителя (номер узла)
                dataSend.append((char) 0x01);                       // 5 байт - флаг кадра ( 0 - я сервер, 1 - я клинет )
                dataSend.append((char) 0x7F);                       // 6 байт - тип кадра 0
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
            //}
        break;
        case command_type::com_package:

        break;
    }
}


void NetworkClient::addCommand(command_t command)  {
    /*
    bufferSend.clear();
    bufferSend.append((command.code >> 24) & 0xFF);
    bufferSend.append((command.code >> 16) & 0xFF);
    bufferSend.append((command.code >> 8)  & 0xFF);
    bufferSend.append((command.code) & 0xFF);

    bufferSend.append((command.par1 >> 24) & 0xFF);
    bufferSend.append((command.par1 >> 16) & 0xFF);
    bufferSend.append((command.par1 >> 8)  & 0xFF);
    bufferSend.append((command.par1) & 0xFF);

    bufferSend.append((command.par2 >> 24) & 0xFF);
    bufferSend.append((command.par2 >> 16) & 0xFF);
    bufferSend.append((command.par2 >> 8)  & 0xFF);
    bufferSend.append((command.par2) & 0xFF);
    */

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


