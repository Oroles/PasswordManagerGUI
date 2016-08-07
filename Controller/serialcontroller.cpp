#include "serialcontroller.h"

#include "Messages/replyaddmessage.h"
#include "Messages/replydeletemessage.h"
#include "Messages/replygetwebsitesmessage.h"
#include "Messages/replyopenportmessage.h"
#include "Messages/replygeneratedaddmessage.h"
#include "Messages/replyisalivemessage.h"
#include "Messages/requestmessage.h"
#include "utils.h"
#include "settings.h"

#include <sstream>
#include <algorithm>

SerialController::SerialController(QObject *parent)
    : QObject(parent)
    , m_serialPort(nullptr)
    , m_isCorrectPort(false)
    , m_isAlivedPort(false)
{
    connect(&m_openPortTimer, &QTimer::timeout, [=]() {
        if ( m_isCorrectPort != true )
        {
            emit displayMessage("Port status", "Try another port", Qt::ApplicationModal);
        }
    });

    connect(&m_isAlivedPortTimer, &QTimer::timeout, [=]() {
        if (Settings::getInstance().IsSendingAlivePackages()) {
            requestIsStillAlive();
            m_isAlivedPort ? replyPortStatus("Connected") : replyPortStatus("Disconnected");
            m_isAlivedPort = false;
        }
    });

    connect(&m_serialReceiver, SIGNAL(receivedMessage(QString)), this, SLOT(receivedReply(QString)));
    m_serialReceiver.start();
}

SerialController::~SerialController()
{
    m_serialReceiver.stop();
}

bool SerialController::openPort(const QString name, int baudRate, int databits, int parity, int stopBits, int flowControl)
{
    try
    {
        m_isCorrectPort = false;
        m_requests.clear();

        replyPortStatus("Disconnected");
        m_isAlivedPortTimer.stop();

        m_serialPort = std::unique_ptr<SerialCommunication>(new SerialCommunication(this, name, baudRate, databits, parity, stopBits, flowControl));
        connect(m_serialPort.get(), SIGNAL(receivedCommand(QString)), &m_serialReceiver, SLOT(add(QString)), Qt::ConnectionType::DirectConnection);

        requestCheckCorrectPort();

        m_openPortTimer.setInterval(1000);
        m_openPortTimer.setSingleShot(true);
        m_openPortTimer.start();

        return true;

    } catch (std::exception&)
    {
        return false;
    }
}

bool SerialController::isPortOpened() const
{
    return m_isCorrectPort;
}

bool SerialController::requestAddEntry(const QString &website, const QString &username, const QString &password) const
{
    if (m_isCorrectPort)
    {
        RequestMessage msg(QStringList{ QString("1"), website, username, password});
        m_requests.append(msg);
        return m_serialPort->sendCommand(msg.toString().toStdString());
    }
    return false;
}

bool SerialController::requestRetrieveEntry(const QString &website, const QString &username) const
{
    if (m_isCorrectPort)
    {
        //do not store the request in the list because we don't get reply
        RequestMessage msg(QStringList{ QString("2"), website, username});
        return m_serialPort->sendCommand(msg.toString().toStdString());
    }
    return false;
}

bool SerialController::requestDeleteEntry(const QString& website, const QString& username) const
{
    if (m_isCorrectPort)
    {
        RequestMessage msg(QStringList{ QString("3"), website, username});
        m_requests.append(msg);
        return m_serialPort->sendCommand(msg.toString().toStdString());
    }
    return false;
}

bool SerialController::requestWebsites() const
{
    if (m_isCorrectPort)
    {
        RequestMessage msg(QStringList{ QString("4")});
        m_requests.append(msg);
        return m_serialPort->sendCommand(msg.toString().toStdString());
    }
    return false;
}

bool SerialController::requestCheckCorrectPort() const
{
    RequestMessage msg(QStringList{ QString("5")});
    m_requests.append(msg);
    return m_serialPort->sendCommand(msg.toString().toStdString());
}

bool SerialController::requestAddEntryAndGenerate(const QString &website, const QString &username, const QString &types, const QString &passwordLength) const
{
    if (m_isCorrectPort)
    {
        RequestMessage msg(QStringList{ QString("7"), website, username, types, passwordLength});
        m_requests.append(msg);
        m_requests.append(RequestMessage(QStringList{QString("1"), website, username}));
        return m_serialPort->sendCommand(msg.toString().toStdString());
    }
    return false;
}

bool SerialController::requestIsStillAlive() const
{
    if (m_isCorrectPort)
    {
        RequestMessage msg(QStringList{ QString("8")});
        m_requests.append(msg);
        return m_serialPort->sendCommand(msg.toString().toStdString());
    }
    return false;
}

void SerialController::receivedReply(const QString& reply)
{
    try
    {
        std::shared_ptr<ReplyMessage> message = this->decodeMessage(reply);
        if (message)
        {
            if (message->isExpected(m_requests))
            {
                message->process();
            }
        }
    } catch(std::exception& ex)
    {
        emit displayMessage("Error", ex.what(), Qt::NonModal);
        emit addToLog("Error");
    }
}

std::shared_ptr<ReplyMessage> SerialController::decodeMessage(const QString& message)
{
    if (!message.isEmpty())
    {
        switch((Utils::ReplyCode)message[0].digitValue())
        {
            case Utils::ReplyCode::ReplyAddEntry:
                return std::shared_ptr<ReplyAddMessage>(new ReplyAddMessage(message, this));
            case Utils::ReplyCode::ReplyDeleteEntry:
                return std::shared_ptr<ReplyDeleteMessage>(new ReplyDeleteMessage(message, this));
            case Utils::ReplyCode::ReplyObtainWebsites:
                return std::shared_ptr<ReplyGetWebsitesMessage>(new ReplyGetWebsitesMessage(message, this));
            case Utils::ReplyCode::ReplyOpenPort:
                return std::shared_ptr<ReplyOpenPortMessage>(new ReplyOpenPortMessage(message, this));
            case Utils::ReplyCode::ReplyPasswordGenerated:
                return std::shared_ptr<ReplyGeneratedAddMessage>(new ReplyGeneratedAddMessage(message, this));
            case Utils::ReplyCode::ReplyIsAlive:
                return std::shared_ptr<ReplyIsAliveMessage>(new ReplyIsAliveMessage(message, this));
            case Utils::ReplyCode::ReplyError:
                return nullptr;
        }
    }
    return nullptr;
}

void SerialController::replyAddMessage(const QString &reply, const QString& website, const QString& username)
{
    if (reply == "OK")
    {
        replyAddEntry(website, username);
        emit displayMessage("Information", "Entry Added: \n" + website + "\n" + username, Qt::NonModal);
        emit addToLog("Entry Added");
    }
    else
    {
        emit displayMessage("Information", "Entry Not Added: \n" + website + "\n" + username, Qt::NonModal);
        emit addToLog("Entry not Added");
    }
}

void SerialController::replyDeleteMessage(const QString &reply, const QString& website, const QString& username)
{
    if (reply == "OK")
    {
        replyDeleteEntry(website, username);
        emit displayMessage("Information", "Entry Deleted: \n" + website + "\n" + username, Qt::NonModal);
        emit addToLog("Entry Deleted");
    }
    else
    {
        emit displayMessage("Information", "Entry Not Deleted: \n" + website + "\n" + username, Qt::NonModal);
        emit addToLog("Entry not Deleted");
    }
}

void SerialController::replyGeneratedAddMessage(const QString &reply)
{
    emit displayMessage("Information", reply, Qt::NonModal);
    emit addToLog(reply);
}

void SerialController::replyOpenPortMessage(const QString&)
{
    m_isCorrectPort = true;
    m_isAlivedPort = true;

    m_isAlivedPortTimer.setInterval(1000);
    m_isAlivedPortTimer.setSingleShot(false);
    m_isAlivedPortTimer.start();
}

void SerialController::replyIsAliveMessage(const QString&)
{
    m_isAlivedPort = true;
}
