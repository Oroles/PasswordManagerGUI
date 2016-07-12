#ifndef REPLYOPENPORTMESSAGE_H
#define REPLYOPENPORTMESSAGE_H

#include "replymessage.h"
#include "requestmessage.h"
#include "Controller/serialcontroller.h"
#include <QObject>

//Private otherwise is nested in ReplyOpenPortMessage
class ReplyOpenPortMessageSender : public QObject
{
    Q_OBJECT
public:
    ReplyOpenPortMessageSender(SerialController* controller);

signals:
    void replyOpenPortMessage(const QString&);
};

class ReplyOpenPortMessage : public ReplyMessage
{
public:
    ReplyOpenPortMessage(const QString& command, SerialController* controller);
    virtual bool isExpected(QList<RequestMessage>& msg) const override;

    virtual bool isValid(const QString& command) const override;
    virtual bool decode(const QString& command, QStringList& arguments) const override;
    virtual void process();

private:
    std::unique_ptr<ReplyOpenPortMessageSender> m_sender;
};

#endif // REPLYOPENPORTMESSAGE_H
