#include "serialcommunication.h"

#include <QFile>
#include <exception>
#include <QDebug>

/*
 * Compress the string that I send, the compress should be one to one,
 * otherwise for two websites there will one entry in database
 * other solution is rangeencoding
 */

bool sendCommand(QSerialPort *port, QString (*ptr_func)())
{
    // creates the command
    QString command = (*ptr_func)();

    // writes the command
    qint64 bytesWritten = port->write(QByteArray(command.toStdString().c_str()));
    port->waitForBytesWritten(3000);

    // checks if the command was written
    return bytesWritten == command.size();
}

bool sendCommand(QSerialPort* port, QString (*ptr_func)(const QString&, const QString&), const QString& arg1, const QString& arg2)
{
    // creates the command
    QString command = (*ptr_func)(arg1,arg2);

    // writes the command
    qint64 bytesWritten = port->write(QByteArray(command.toStdString().c_str()));
    port->waitForBytesWritten(3000);

    // checks if the command was written
    return bytesWritten == command.size();
}

bool sendCommand(QSerialPort *port, QString (*ptr_func)(const QString&, const QString&, const QString&), const QString& arg1, const QString& arg2, const QString& arg3)
{
    // creates the ccomand
    QString command = (*ptr_func)(arg1,arg2,arg3);

    // writes the command
    qint64 bytesWritten = port->write(QByteArray(command.toStdString().c_str()));
    port->waitForBytesWritten(3000);

    // checks if the command was written
    return bytesWritten == command.size();
}

SerialCommunication* SerialCommunication::getInstance()
{
    //returns a null pointer if connection could not be open
    try
    {
        static SerialCommunication serialCommunication;
        return &serialCommunication;
    }
    catch(...)
    {
        return nullptr;
    }
}

SerialCommunication::SerialCommunication(QObject *parent) :
    QObject(parent),
    serialPort(nullptr)
{
    this->readConfiguration();

    // read data from a file and updates the serial port
    serialPort = new QSerialPort(m_portName);

    // open the port
    if (serialPort->open(QIODevice::ReadWrite))
    {
        // set configuration from the file config
        serialPort->setBaudRate(m_baudRate);
        serialPort->setDataBits(m_dataBits);
        serialPort->setParity(m_parity);
        serialPort->setStopBits(m_stopBits);
        serialPort->setFlowControl(m_flowControl);
        qDebug() << "Port opened";
    }
    else
    {
        throw std::exception();
    }

    // make the connections
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(readBytes()));
}

SerialCommunication::~SerialCommunication()
{
    // if the port is open close it
    if (serialPort != nullptr)
    {
        if (serialPort->isOpen())
        {
            serialPort->close();
        }
        delete serialPort;
    }
}

bool SerialCommunication::checkPasswordAndUser(const QString& username,const QString& password) const
{
    // checks if the port is open then send the command, the reply will be on the function readBytes
    if (serialPort->isOpen())
    {
        return sendCommand(serialPort, Utils::encodeRequestCheckPasswordAndUser, username, password);
    }
    return false;
}

bool SerialCommunication::writeNewAccount(const QString& username, const QString& password) const
{
    // checks if the port is open then send the command, the reply will be on the function readBytes
    if (serialPort->isOpen())
    {
        return sendCommand(serialPort, Utils::encodeRequestWriteNewAccount,username,password);
    }
    return false;
}

bool SerialCommunication::deletePasswordAndUser() const
{
    // checks if the port is open then send the command, the reply will be on the function readBytes
    if (serialPort->isOpen())
    {
        return sendCommand(serialPort, Utils::encodeRequestDeletePasswordAndUser);
    }
    return false;
}

bool SerialCommunication::addEntry(const QString& website, const QString& username, const QString& password) const
{
    // checks if the port is open then send the command, the reply will be on the function readBytes
    if (serialPort->isOpen())
    {
        return sendCommand(serialPort, Utils::encodeRequestAddEntry, website, username, password);
    }
    return false;

}
bool SerialCommunication::retriveEntry(const QString& website, const QString& username) const
{
    // checks if the port is open then send the command, the reply will be on the function readBytes
    if (serialPort->isOpen())
    {
        return sendCommand(serialPort, Utils::encodeRequestRetriveEntry, website, username);
    }
    return false;
}

bool SerialCommunication::deleteEntry(const QString& website, const QString& username) const
{
    // checks if the port is open then send the command, the reply will be on the function readBytes
    if (serialPort->isOpen())
    {
        return sendCommand(serialPort, Utils::encodeRequestDeleteEntry, website, username);
    }
    return false;
}

bool SerialCommunication::obtainWebsites()
{
    // checks if the port is open then send the command, the reply will be on the function readBytes
    if (serialPort->isOpen())
    {
        return sendCommand(serialPort, Utils::encodeRequestObtainWebsites);
    }
    return false;
}

void SerialCommunication::readBytes()
{
    static QString buffer = "";

    // read bytes and store them in buffer
    buffer.append(serialPort->readAll() );

    // check if there is a command in the buffer
    int indexOfNewLine = buffer.indexOf("\n");
    if (indexOfNewLine != -1)
    {
        // there is a command in buffer

        // get the command & remove command from buffer
        QString command = buffer.left(indexOfNewLine);
        qDebug() << "Command: " << command;
        buffer.clear();
        qDebug() << "Buffer: " << buffer;

        if(command == "")
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
                emit sendMessageToMain(Utils::ReplyCode::ReplyAddEntry, "Add Entry return command: ", arg1);
                break;
            case Utils::ReplyCode::ReplyCheckPasswordAndUser:
                emit sendMessageToLogin("Check Password and User: ", arg1);
                break;
            case Utils::ReplyCode::ReplyDeleteEntry:
                emit sendMessageToMain(Utils::ReplyCode::ReplyDeleteEntry, "Delete Entry: ", arg1);
                break;
            case Utils::ReplyCode::ReplyDeletePasswordAndUser:
                emit sendMessageToLogin("Delete Password And User: ", arg1);
                break;
            case Utils::ReplyCode::ReplyObtainWebsites:
                emit sendNewWebsite(arg1, arg2);
                break;
            case Utils::ReplyCode::ReplyRetriveEntry:
                emit sendPassword(arg1);
                break;
            case Utils::ReplyCode::ReplyWriteNewAccount:
                emit sendMessageToSingUp("New account: ", arg1);
                break;
            case Utils::ReplyCode::ReplyError:
                emit sendMessageToMain(Utils::ReplyCode::ReplyError, "Something wrong: ","Error");
                break;
        }
    }
}

bool SerialCommunication::readConfiguration()
{
    /*Reads the configuration for the port
     *from the file. Return true is everything
     *could be read, return false otherwhise
     */
    static bool alreadyRead = false;
    if (!alreadyRead)
    {
        alreadyRead = true;
        QFile configurationFile("config");

        // tests if the file was found
        if(!configurationFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return false;
        }

        // reads each line in memory
        QList<QByteArray> lines;
        while (!configurationFile.atEnd())
        {
            lines.append(configurationFile.readLine());
        }

        // updates serial port
        m_portName = Utils::getName("Name", lines);
        m_baudRate = static_cast<QSerialPort::BaudRate>(Utils::getValue("BaudRate", lines));
        m_dataBits = static_cast<QSerialPort::DataBits>(Utils::getValue("DataBits", lines));
        m_parity = static_cast<QSerialPort::Parity>(Utils::getValue("Parity", lines));
        m_stopBits = static_cast<QSerialPort::StopBits>(Utils::getValue("StopBits", lines));
        m_flowControl = static_cast<QSerialPort::FlowControl>(Utils::getValue("FlowControl", lines));
        return true;
    }
    return false;
}
