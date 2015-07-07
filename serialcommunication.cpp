#include "serialcommunication.h"
#include "utils.h"

#include <QFile>

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
    QObject(parent)
{
    // read data from a file and updates the serial port
    serialPort = new QSerialPort(this);
    this->readConfiguration();

    // TODO open the port

    // TODO set port configuration

    // make the connections
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(readBytes()));
}

SerialCommunication::~SerialCommunication()
{
    // if the port is open close it
    if (serialPort->isOpen())
    {
        serialPort->close();
    }
}

bool SerialCommunication::checkPasswordAndUser(QString username, QString password) const
{
    //TODO implement functionality
    return true;
}

bool SerialCommunication::writeNewAccount(QString username, QString password) const
{
    //TODO implement functionality
    return true;
}

bool SerialCommunication::deletePasswordAndUser() const
{
    //TODO implement functionality
    return true;
}

bool SerialCommunication::addEntry(QString website, QString username, QString password) const
{
    //TODO implement functionality
    return true;
}
bool SerialCommunication::retriveEntry(QString website, QString username) const
{
    //TODO implement functionality
    return true;
}

bool SerialCommunication::deleteEntry(QString website, QString username) const
{
    //TODO implement functionality
    return true;
}

void SerialCommunication::obtainWebsites()
{
    //TODO implement functionality

    // should send a command to the board to obtain all the websites
}

void SerialCommunication::readBytes()
{
    //TODO implement functionality
    //1. read bytes
    //2. deserializeaza bytes
    //3. construieste comanda
    //4. executa comanda
}

bool SerialCommunication::readConfiguration()
{
    /*Reads the configuration for the port
     *from the file. Return true is everything
     *could be read, return false otherwhise
     */
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
