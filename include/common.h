#pragma once
#include <QDataStream>
#include <QList>

// 8 byte param for command that supports "double"
struct STR_PARAM    {
public:
    QByteArray data;
	STR_PARAM(QByteArray arr)	{
		data.clear();
		data.append(arr);
	}
	STR_PARAM() {}	
    STR_PARAM(uint value) {
        data.clear();
		qint64 v = (qint64) value;
        data.append(static_cast<char>((v >> 56) & 0xFF));
        data.append(static_cast<char>((v >> 48) & 0xFF));
        data.append(static_cast<char>((v >> 40) & 0xFF));
        data.append(static_cast<char>((v >> 32) & 0xFF));
        data.append(static_cast<char>((v >> 24) & 0xFF));
        data.append(static_cast<char>((v >> 16) & 0xFF));
        data.append(static_cast<char>((v >> 8) & 0xFF));
        data.append(static_cast<char>((v >> 0) & 0xFF));
    }
    STR_PARAM(qint32 value) {
        data.clear();
		qint64 v = (qint64) value;
        data.append(static_cast<char>((v >> 56) & 0xFF));
        data.append(static_cast<char>((v >> 48) & 0xFF));
        data.append(static_cast<char>((v >> 40) & 0xFF));
        data.append(static_cast<char>((v >> 32) & 0xFF));
        data.append(static_cast<char>((v >> 24) & 0xFF));
        data.append(static_cast<char>((v >> 16) & 0xFF));
        data.append(static_cast<char>((v >> 8) & 0xFF));
        data.append(static_cast<char>((v >> 0) & 0xFF));
    }
    STR_PARAM(qint64 value) {
        data.clear();
        data.append(static_cast<char>((value >> 56) & 0xFF));
        data.append(static_cast<char>((value >> 48) & 0xFF));
        data.append(static_cast<char>((value >> 40) & 0xFF));
        data.append(static_cast<char>((value >> 32) & 0xFF));
        data.append(static_cast<char>((value >> 24) & 0xFF));
        data.append(static_cast<char>((value >> 16) & 0xFF));
        data.append(static_cast<char>((value >> 8) & 0xFF));
        data.append(static_cast<char>((value >> 0) & 0xFF));
    }
    STR_PARAM(double value) {
        data.clear();
        QDataStream stream(&data,QIODevice::WriteOnly);
        stream << value;
    }
    STR_PARAM(char value)   {
        data.clear();
        data.append(static_cast<char>(0x00));
        data.append(static_cast<char>(0x00));
        data.append(static_cast<char>(0x00));
        data.append(static_cast<char>(0x00));
        data.append(static_cast<char>(0x00));
        data.append(static_cast<char>(0x00));
        data.append(static_cast<char>(0x00));
        data.append(value);
    }
    qint64 toInt()   {
        qint64 result;
        QDataStream stream(&data, QIODevice::ReadOnly);
        stream >> result;
        return result;
    }
    double toDouble()   {
        double result;
        QDataStream stream(&data, QIODevice::ReadOnly);
        stream >> result;
        return result;
    }
    char toChar()   {
        char ch = *data.mid(7,1).data();
        return ch;
    }
};

// template command for models and formats
struct command_t {
    uint code;
    STR_PARAM par1;
    STR_PARAM par2;
    qint32 time;
};

// template command for na to STR
struct command_na {
    uint code;
    qint32 par1;
    qint32 par2;
    qint32 time;
};

// template package for na to STR
struct package_na {
    qint32 code;
    QByteArray data;
};

// list of socket commands
enum network_command_type   {
    com_init,
    com_buffer,
    com_ping,
    com_package,
    com_na
};

// connecting of commands and model owners
struct STRCommand {
    uint code;
    qint32 model_owner;
    QString description;
};

// command info for event monitor
struct EventCommand	{
	uint code;
	qint32 time;
};

// format info for container
template <typename T>
struct STRFormat	{
    qint32 id;
    T name;
    QString title;
    QString object;
    QString system;
};

// format list in system info for container
template <typename T>
struct STRFormatSystemContainer {
    QString title;
    QList <STRFormat<T>> FormatContainer;
};

// system list in object info for container
template <typename T>
struct STRFormatObjectContainer {
    QString title;
    QList <STRFormatSystemContainer<T>> SystemContainer;

};

// node description
struct STRNode  {
    QString name;
    QString host;
	bool isServer;
    qint32 port;
    qint32 ID;
	qint32 mainNode;
    qint32 frameType;
	QString modelName;
};

// node info and state for state format
struct STRNodeInfo	{
	QString name;
	qint32 ID;
	qint32 state;	// 0 - disconnected, 1 - connected, 2 - lost connection
	bool isNA;
};
