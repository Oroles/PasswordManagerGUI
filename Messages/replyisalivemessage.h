#ifndef REPLYISALIVEMESSAGE_H
#define REPLYISALIVEMESSAGE_H

#include "replymessage.h"
#include "Controller/serialcontroller.h"
#include <QObject>

//Private otherwise is nested in ReplyIsAliveMessage
class ReplyIsAliveMessageSender : public QObject
{
    Q_OBJECT
public:
    ReplyIsAliveMessageSender(SerialController* controller);

signals:
    void replyIsAliveMessage(const QString&);
};

class ReplyIsAliveMessage : public ReplyMessage
{
public:
    ReplyIsAliveMessage(const QString& command, SerialController* controller);
    virtual bool isExpected(QList<RequestMessage>& msg) const override;

    virtual bool isValid(const QString& command) const override;
    virtual bool decode(const QString& command, QStringList& arguments) const override;
    virtual void process();

private:
    std::unique_ptr<ReplyIsAliveMessageSender> m_sender;
};

#endif // REPLYISALIVEMESSAGE_H
