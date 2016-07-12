#include "serialcommunication.h"

#include <QFile>
#include <exception>
#include <QDebug>

bool sendCommandImpl(std::shared_ptr<QSerialPort> port, const std::string& command)
{
    // writes the command
    qint64 bytesWritten = port->write(QByteArray(command.c_str()));
    port->waitForBytesWritten(3000);

    if (bytesWritten == -1) {
        return false;
    }

    // checks if the command was written
    return bytesWritten == command.size();
}

SerialCommunication::SerialCommunication(QObject* parent, const QString name, int baudRate, int dataBits, int parity, int stopBits, int flowControl) :
    QObject(parent),
    serialPort(nullptr)
{
    serialPort = std::make_shared<QSerialPort>(name);

    // open the port
    if (serialPort->open(QIODevice::ReadWrite) && serialPort->isOpen())
    {
        // set configuration from the file config
        serialPort->setBaudRate(static_cast<QSerialPort::BaudRate>(baudRate));
        serialPort->setDataBits(static_cast<QSerialPort::DataBits>(dataBits));
        serialPort->setParity(static_cast<QSerialPort::Parity>(parity));
        serialPort->setStopBits(static_cast<QSerialPort::StopBits>(stopBits));
        serialPort->setFlowControl(static_cast<QSerialPort::FlowControl>(flowControl));
    }
    else
    {
        throw std::exception();
    }

    // make the connections
    connect(serialPort.get(), SIGNAL(readyRead()), this, SLOT(readBytes()));
}

SerialCommunication::~SerialCommunication()
{
    this->closeSerialPort();
}

void SerialCommunication::closeSerialPort()
{
    // if the port is open close it
    if (serialPort != nullptr)
    {
        if (serialPort->isOpen())
        {
            serialPort->close();
            disconnect(serialPort.get(), SIGNAL(readyRead()), this, SLOT(readBytes()));
        }
    }
}

bool SerialCommunication::sendCommand(const std::string &command)
{
    return sendCommandImpl(serialPort, command);
}

void SerialCommunication::readBytes()
{
    static QString buffer = "";

    // read bytes and store them in buffer
    buffer.append(serialPort->readAll());

    // check if there is a command in the buffer
    int indexOfNewLine = buffer.indexOf( Utils::END_COMMAND);
    while (indexOfNewLine != -1) // in case there are multiple commands read
    {

        // get the command
        QString command = buffer.left(indexOfNewLine);
        QString tmp = buffer;
        buffer = buffer.right( buffer.size() - indexOfNewLine - 1 ); // -1 to remove the indexOfNewLine
        if ( tmp == buffer ) {
            buffer = "";
        }
        qDebug() << "Command: " << command;
        qDebug() << "Buffer: " << buffer;

        receivedCommand(command);

        indexOfNewLine = buffer.indexOf( Utils::END_COMMAND);
    }
}
