#include "network_server.h"

NetworkServer::NetworkServer(qint32 Port)
{
    m_server = new QTcpServer(this);

    if(m_server->listen(QHostAddress::Any, Port))   {
       connect(m_server, &QTcpServer::newConnection,
               this, &NetworkServer::newConnection);
        qDebug() << "System :: server is listening...";
    } else {
        qDebug() << QString("System :: unable to start server. Code = %1").arg(m_server->errorString());
        exit(EXIT_FAILURE);
    }

    m_timer = new QTimer;
    m_timer->setInterval(500);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(OnTimer()));
    m_timer->start();

    bInit = false;
}

NetworkServer::~NetworkServer()
{
    foreach (QTcpSocket* socket, connection_set)    {
        socket->disconnectFromHost();
        socket->close();
        socket->deleteLater();
    }
    m_server->close();
    m_server->deleteLater();
    m_timer->stop();
    delete m_timer;
}

void NetworkServer::addCommand(command_t command)
{
    qDebug() << QString("Sending command: %1 %2 %3").arg(command.code).arg(command.par1.toInt()).arg(command.par2.toInt());
    for(auto sock : connection_set) {
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

        sendMessage(sock, command_type::com_buffer);
    }
}

void NetworkServer::addPackage(QByteArray package)
{
    //qDebug() << QString("Sending core package");
    //qDebug() << package;
    for(auto sock : connection_set) {
        bufferSend.clear();
        bufferSend.append(package);
        sendMessage(sock, command_type::com_package);
    }
}

void NetworkServer::OnTimer()
{
    for(auto sock : connection_set) {
        sendMessage(sock, command_type::com_ping);
    }
}

void NetworkServer::newConnection()    {
    while (m_server->hasPendingConnections())   {
        appendToSocketList(m_server->nextPendingConnection());
    }
}

void NetworkServer::appendToSocketList(QTcpSocket* socket) {
    connection_set.insert(socket);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkServer::readyRead);
    connect(socket, &QTcpSocket::disconnected, this, &NetworkServer::discardSocket);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(getErrorCode(QAbstractSocket::SocketError)));
    qDebug() << QString("Node %1 descriptor has connected").arg(socket->socketDescriptor());
    emit signalClientsAreConnected(connection_set.size());
}

void NetworkServer::discardSocket()    {
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QSet<QTcpSocket*>::iterator it = connection_set.find(socket);
    if (it != connection_set.end()){
        qDebug() << QString("Node has disconnected").arg(socket->socketDescriptor());
        connection_set.remove(*it);
    }
    socket->deleteLater();
    emit signalClientsAreConnected(connection_set.size());
}

void NetworkServer::getErrorCode(QAbstractSocket::SocketError errorCode)   {
    qDebug() << "Socket Error = " << errorCode;
}

void NetworkServer::readyRead() {

    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    if( socket->state() == QAbstractSocket::ConnectedState )    {

        bufferRead = socket->readAll();

        //qDebug() << "Length: " << bufferRead.length();
        //qDebug() << bufferRead;

        if(bInit == false) {

            int MainNode  = 2;
            int NodeID    = 1;
            int FrameType = 0x7F;
            QString name  = "127.0.0.1";

            dataSend.clear();

            dataSend.append((char) 0x00);
            dataSend.append((char) 13 + name.size());
            dataSend.append((char) MainNode);
            dataSend.append((char) NodeID);
            dataSend.append((char) 0x00);
            dataSend.append((char) FrameType);
            dataSend.append((char) 0x00);
            dataSend.append((char) 0x00);

            dataSend.append((char) 0xC0);
            dataSend.append((char) 0x00);
            dataSend.append((char) 0x00);
            dataSend.append((char) 0x03);

            dataSend.append(name);
            dataSend.append((char) 0x00);

            bInit = true;

            qDebug() << "Init command is sent";

            socket->write(dataSend);
            socket->flush();
            socket->waitForBytesWritten(10000);
        }

        if(bufferRead.length() >= 32)   {
            bool status;
            QByteArray header;
            header = bufferRead.mid(0,8);
            int iSize = header.mid( 1, 1).toHex().toInt(&status, 16);
            int iMainNode = header.mid( 2, 1).toHex().toInt(&status, 16);
            int iClientNode = header.mid( 3, 1).toHex().toInt(&status, 16);
            int iNetworkFlag = header.mid( 4, 1).toHex().toInt(&status, 16);
            int iFrameType = header.mid( 5, 1).toHex().toInt(&status, 16);

            if(iNetworkFlag == 0)   {
                qDebug() << "Have got package from server : " << iClientNode;
            } else {
                qDebug() << "Have got package from client: " << iClientNode;
            }
            qDebug() << "Size = " << iSize << " FrameType = " << iFrameType;

                // checking for default command
            if(iSize == 32) {
                QByteArray frame;
                frame = bufferRead.mid(0,32);
                frame.remove(0,8);

                if(frame.length() >= 24)    {                   // checking if frame is correct
                    bool status;
                    uint iCommand = frame.mid( 0,4).toHex().toUInt(&status,16);

                    STR_PARAM p1 = STR_PARAM(frame.mid( 4,8));
                    STR_PARAM p2 = STR_PARAM(frame.mid( 12,8));

                    //int iPar1    = frame.mid( 4,4).toHex().toInt(&status,16);
                    //int iPar2    = frame.mid( 8,4).toHex().toInt(&status,16);
                    qint32 iTime    = frame.mid(20,4).toHex().toInt(&status,16);
                    bufferRead.remove(0, 32);                   // delete frame from buffer

                    qDebug() << "Input command : " << iCommand << " " << p1.toInt() << " " << p2.toInt() << " " << iTime;
                    emit signalCommandFromClient({iCommand, p1, p1, iTime});
                }
            } else {
                // try to unpack big package
            }
        }
    }
}

void NetworkServer::sendMessage(QTcpSocket* socket, command_type type)    {
    switch(type)    {
        case com_init:
            if(socket)  {
                if(socket->isOpen())    {

                    QDataStream socketStream(socket);
                    socketStream.setVersion(QDataStream::Qt_4_0);

                    int MainNode  = 2;
                    int NodeID    = 1;
                    int FrameType = 0x7F;
                    int Port      = 9999;

                    QString name = "127.0.0.1";

                    QByteArray byteArray;

                    byteArray.append((char) 0x00);
                    byteArray.append((char) 13 + name.size());
                    byteArray.append((char) MainNode);
                    byteArray.append((char) NodeID);
                    byteArray.append((char) 0x00);
                    byteArray.append((char) 0x7F);
                    byteArray.append((char) 0x00);
                    byteArray.append((char) 0x00);

                    byteArray.append((char) 0xC0);
                    byteArray.append((char) 0x00);
                    byteArray.append((char) 0x00);
                    byteArray.append((char) 0x03);

                    byteArray.append(name);
                    byteArray.append((char) 0x00);

                    socketStream << byteArray;
                    socket->write(byteArray);
                    socket->flush();
                    qDebug() << "Init command is sent";
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
            dataSend.append((char) 0x00);           // 5 байт - флаг кадра ( 0 - я сервер, 1 - я клинет )
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
                dataSend.append((char) 0x00);                       // 5 байт - флаг кадра ( 0 - я сервер, 1 - я клинет )
                dataSend.append((char) 0x7F);                       // 6 байт - тип кадра 0
                dataSend.append((char) 0x00);                       // 7 байт - зарезервировано
                dataSend.append((char) 0x00);                       // 8 байт - зарезервировано

                // 4 байта - модельное время команды в миллисекундах
                bufferSend.append((char) 0x00);
                bufferSend.append((char) 0x00);
                bufferSend.append((char) 0x00);
                bufferSend.append((char) 0x00);
                // модельное время команды в миллисекундах

                //qDebug() << "bufferSend.length()" << bufferSend.length() << "bufferSend" << bufferSend;
                dataSend.append(bufferSend);
                //dataSend[1] = (char) 0x18;     // меняем длинну кадра на 24
                //dataSend[5] = (char) 0x7F;  // меняем тип кадра на 126
                bufferSend.clear(); // очищаем буфер
            //}

            socket->write(dataSend);
            socket->flush();
            socket->waitForBytesWritten(10000);
        break;
        case com_package:

            dataSend.clear();

            int iPackSize = bufferSend.size();

            // 8 байт - заголовок кадра
            dataSend.append((char) 0x01);           // 1 байт - признак пакета для синхронизации ядра
            dataSend.append((char) 0xFF);           // 2 байт - текущая длинна кадра в байтах (в пакете не используется)
            dataSend.append((char) 0x02);           // 3 байт - идентификатор получателя
            dataSend.append((char) 0x01);           // 4 байт - идентификатор отправителя (номер узла)
            dataSend.append((char) 0x00);           // 5 байт - флаг кадра ( 0 - я сервер, 1 - я клинет )
            dataSend.append((char) 0x7F);           // 6 байт - тип кадра 0
            dataSend.append((char) 0x00);           // 7 байт - зарезервировано
            dataSend.append((char) 0x00);           // 8 байт - зарезервировано

            // 4 байта - размер пакета ядра
            bufferSend.prepend((iPackSize >> 24) & 0xFF);
            bufferSend.prepend((iPackSize >> 16) & 0xFF);
            bufferSend.prepend((iPackSize >> 8)  & 0xFF);
            bufferSend.prepend((iPackSize) & 0xFF);

            dataSend.append(bufferSend);

            qDebug() << "dataSend.length()" << dataSend.length();// << "bufferSend" << bufferSend;
            //qDebug() << dataSend;
            bufferSend.clear(); // очищаем буфер


            socket->write(dataSend);
            socket->flush();
            socket->waitForBytesWritten();
        break;
    }
}















