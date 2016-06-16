#include "serialcommunication.h"

#include <QFile>
#include <exception>
#include <sstream>
#include <QDebug>

bool sendCommand(std::shared_ptr<QSerialPort> port, const std::string& command)
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
    m_isPortOpened(false),
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

    //set the connection of the timer
    connect(&m_openPortTimer, &QTimer::timeout, [=]() {
        if ( !m_isPortOpened )
        {
            emit sendMessageToMain(Utils::ReplyCode::ReplyCorrectPort, "Port status:", "Try another port");
        }
    });

    m_openPortTimer.setInterval(1000);
    m_openPortTimer.setSingleShot(true);
    m_openPortTimer.start();
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

bool SerialCommunication::addEntry( const QString& website, const QString& username, const QString& password) const
{
    // checks if the port is open then send the command, the reply will be on the function readBytes
    if (serialPort->isOpen())
    {
        std::stringstream stream;
        Utils::generateRequest(stream, QString("1"), website, username, password);
        return sendCommand(serialPort, stream.str());
    }
    return false;
}

bool SerialCommunication::addEntryAndGenerate(const QString &website, const QString &username, const QString &types, const QString &passwordLength) const
{
    if (serialPort->isOpen())
    {
        std::stringstream stream;
        Utils::generateRequest(stream, QString("7"), website, username, types, passwordLength);
        return sendCommand(serialPort, stream.str());
    }
    return false;
}

bool SerialCommunication::retrieveEntry(const QString& website, const QString& username) const
{
    // checks if the port is open then send the command, the reply will be on the function readBytes
    if (serialPort->isOpen())
    {
        std::stringstream stream;
        Utils::generateRequest(stream, QString("2"), website, username);
        return sendCommand(serialPort, stream.str());
    }
    return false;
}

bool SerialCommunication::deleteEntry(const QString& website, const QString& username) const
{
    // checks if the port is open then send the command, the reply will be on the function readBytes
    if (serialPort->isOpen())
    {
        std::stringstream stream;
        Utils::generateRequest(stream, QString("3"), website, username);
        return sendCommand(serialPort, stream.str());
    }
    return false;
}

bool SerialCommunication::obtainWebsites()
{
    // checks if the port is open then send the command, the reply will be on the function readBytes
    if (serialPort->isOpen())
    {
        std::stringstream stream;
        Utils::generateRequest(stream, QString("4"));
        return sendCommand(serialPort, stream.str());
    }
    return false;
}

bool SerialCommunication::checkCorrectPort()
{
    // check if the port is open then sent the command, the reply will be on the function readBytes
    if (serialPort->isOpen())
    {
        std::stringstream stream;
        Utils::generateRequest(stream, QString("5"));
        return sendCommand(serialPort, stream.str());
    }
    return false;
}

bool SerialCommunication::isPortOpened()
{
    return m_isPortOpened;
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
        // there is a command in buffer

        // get the command
        QString command = buffer.left(indexOfNewLine);
        QString tmp = buffer;
        buffer = buffer.right( buffer.size() - indexOfNewLine - 1 ); // -1 to remove the indexOfNewLine
        if ( tmp == buffer ) {
            buffer = "";
        }
        qDebug() << "Command: " << command;
        qDebug() << "Buffer: " << buffer;

        if(!Utils::isValidCommand(command))
        {
            // there was something wrong
            return;
        }

        // decode the command & execute it
        QString arg1,arg2;
        switch ( Utils::decodeReply(command, arg1, arg2) )
        {
            // based on the type of reply send different type of messages
            // to the GUI
            case Utils::ReplyCode::ReplyAddEntry:
                emit sendMessageToMain(Utils::ReplyCode::ReplyAddEntry, "Add Entry: ", arg1);
                break;
            case Utils::ReplyCode::ReplyDeleteEntry:
                emit sendMessageToMain(Utils::ReplyCode::ReplyDeleteEntry, "Delete Entry: ", arg1);
                break;
            case Utils::ReplyCode::ReplyObtainWebsites:
                emit sendNewWebsite(arg1, arg2);
                break;
            case Utils::ReplyCode::ReplyPasswordGenerated:
                emit sendMessageToMain(Utils::ReplyCode::ReplyPasswordGenerated, "Password generated:", arg1);
                break;
            case Utils::ReplyCode::ReplyCorrectPort:
                m_isPortOpened = true;
                emit sendMessageToMain(Utils::ReplyCode::ReplyCorrectPort, "Port status:", arg1);
                break;
            case Utils::ReplyCode::ReplyError:
                emit sendMessageToMain(Utils::ReplyCode::ReplyError, "Something wrong: ","Error");
                break;
            default:
                qDebug() << "Error unknown type\n";
        }
    }
}
