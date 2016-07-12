#ifndef SERIALCONTROLLER_H
#define SERIALCONTROLLER_H

#include <QObject>
#include <QThread>
#include <memory>

#include "Messages/replymessage.h"
#include "Messages/requestmessage.h"
#include "Model/serialcommunication.h"
#include "Model/syncqueue.hpp"

#include "Controller/serialreceiver.h"

class SerialController : public QObject
{
    Q_OBJECT
public:
    SerialController(QObject* parent = 0);
    ~SerialController();

    bool openPort(const QString name, int baudRate, int databits, int parity, int stopBits, int flowControl);

    // Requests
    bool requestAddEntry(const QString &website, const QString &username, const QString &password) const;
    bool requestAddEntryAndGenerate(const QString &website, const QString &username, const QString &types, const QString &passwordLength) const;
    bool requestRetrieveEntry(const QString &website, const QString &username) const;
    bool requestDeleteEntry(const QString& website, const QString& username) const;
    bool requestWebsites() const;
    bool requestCheckCorrectPort() const;
    bool requestIsStillAlive() const;

    bool isPortOpened() const;

signals:
    // signal towards the main window
    void replyAddEntry(const QString& website, const QString& username);
    void replyDeleteEntry(const QString& website, const QString& username);
    void replyGetWebsite(const QString& website, const QString& username, long date);
    void replyPortStatus(const QString& status);
    void addToLog(const QString& message);
    void displayMessage(const QString& title, const QString& message, Qt::WindowModality modal);

public slots:
    // slots from serial communication
    void receivedReply(const QString &reply);

    // slots from messages
    void replyAddMessage(const QString& reply, const QString &website, const QString &username);
    void replyDeleteMessage(const QString& reply, const QString &website, const QString &username);
    void replyGeneratedAddMessage(const QString& reply);
    void replyIsAliveMessage(const QString&);
    void replyOpenPortMessage(const QString &);

private:
    std::shared_ptr<ReplyMessage> decodeMessage(const QString& message);
    void processReplies();

private:
    std::unique_ptr<SerialCommunication> m_serialPort;
    QTimer m_openPortTimer;
    bool m_isCorrectPort;

    QTimer m_isAlivedPortTimer;
    bool m_isAlivedPort;

    mutable QList<RequestMessage> m_requests;

    SerialReceiver m_serialReceiver;

};

#endif // SERIALCONTROLLER_H
