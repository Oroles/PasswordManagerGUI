#include "serialcommunication.h"

#include <QFile>
#include <exception>
#include <QDebug>

/* ./serialcommunication.cpp:70:  [2] (misc) open:
  Check when opening files - can an attacker redirect it (via symlinks),
  force the opening of special file type (e.g., device files), move
  things around to create a race condition, control its ancestors, or change
  its contents?.
  -- put a file name in config file so it will try to write to it -- doesn't open it
  -- try to create a symbolic link from /dev/ttyACM0 -> to a file -- doesn't work or I do something wrong,
                                                                  -- maybe I have to delete one serial port and replace it with another one
                                                                  -- add user to dialout so I can change the ports

   To resolve this I can send and wait a basic reply that everthing works well

./serialcommunication.cpp:205:  [2] (misc) open:
  Check when opening files - can an attacker redirect it (via symlinks),
  force the opening of special file type (e.g., device files), move
  things around to create a race condition, control its ancestors, or change
  its contents?.

./mainwindow.cpp:236:  [4] (shell) system:
  This causes a new program to execute and is difficult to use safely.
  try using a library call that implements the same functionality if
  available. -- se comment at the method void MainWindow::displayPassword(QString password)
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

bool sendCommand(QSerialPort *port, QString (*ptr_func)(const QString&, const QString&, const QString&, const QString&), const QString& arg1, const QString& arg2, const QString& arg3, const QString arg4)
{
    // creates the ccomand
    QString command = (*ptr_func)(arg1,arg2,arg3,arg4);

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
    if (serialPort->open(QIODevice::ReadWrite) && serialPort->isOpen())
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
    connect(serialPort, SIGNAL(aboutToClose()), this, SLOT(aboutToClose()));
}

SerialCommunication::~SerialCommunication()
{
    this->closeSerialPort();
    delete serialPort;
}

void SerialCommunication::closeSerialPort()
{
    // if the port is open close it
    if (serialPort != nullptr)
    {
        if (serialPort->isOpen())
        {
            disconnect(serialPort, SIGNAL(readyRead()), this, SLOT(readBytes()));
            disconnect(serialPort, SIGNAL(aboutToClose()), this, SLOT(aboutToClose()));

            serialPort->close();
        }
    }
}

bool SerialCommunication::addEntry(const QString& website, const QString& username, const QString& password, const QString &key) const
{
    // checks if the port is open then send the command, the reply will be on the function readBytes
    if (serialPort->isOpen())
    {
        return sendCommand(serialPort, Utils::encodeRequestAddEntry, website, username, password, key);
    }
    return false;

}
bool SerialCommunication::retrieveEntry(const QString& website, const QString& username, const QString& key) const
{
    // checks if the port is open then send the command, the reply will be on the function readBytes
    if (serialPort->isOpen())
    {
        return sendCommand(serialPort, Utils::encodeRequestRetrieveEntry, website, username, key);
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

bool SerialCommunication::closeBluetoothConnection()
{
    if(serialPort->isOpen())
    {
        return sendCommand(serialPort, Utils::encodeRequestCloseBluetoothConnection);
    }
    return false;
}

void SerialCommunication::readBytes()
{
    static QString buffer = "";

    // read bytes and store them in buffer
    buffer.append(serialPort->readAll());

    // check if there is a command in the buffer
    int indexOfNewLine = buffer.indexOf("\n");
    while (indexOfNewLine != -1) // in case there are multiple commands read
    {
        // there is a command in buffer

        // get the command
        QString command = buffer.left(indexOfNewLine);
        buffer = buffer.right( buffer.size() - indexOfNewLine - 1 ); // -1 to remove the indexOfNewLine
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
                emit sendMessageToMain(Utils::ReplyCode::ReplyAddEntry, "Add Entry return command: ", arg1);
                break;
            case Utils::ReplyCode::ReplyDeleteEntry:
                emit sendMessageToMain(Utils::ReplyCode::ReplyDeleteEntry, "Delete Entry: ", arg1);
                break;
            case Utils::ReplyCode::ReplyObtainWebsites:
                emit sendNewWebsite(arg1, arg2);
                break;
            case Utils::ReplyCode::ReplyRetrieveEntry:
                emit sendPassword(arg1);
                break;
            case Utils::ReplyCode::CloseConnection:
                this->closeSerialPort();
                break;
            case Utils::ReplyCode::ReplyError:
                emit sendMessageToMain(Utils::ReplyCode::ReplyError, "Something wrong: ","Error");
                break;
        }
    }
}

void SerialCommunication::aboutToClose()
{
   if (!this->closeBluetoothConnection()){
       emit sendMessageToMain(Utils::ReplyCode::ReplyError, "Something wrong: ","Can not close connection");
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
