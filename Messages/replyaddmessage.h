#ifndef ADDMESSAGE_H
#define ADDMESSAGE_H

#include "replymessage.h"
#include "requestmessage.h"
#include "Controller/serialcontroller.h"
#include <QObject>

//Private otherwise is nested in ReplyAddMessage
class ReplyAddMessageSender : public QObject
{
    Q_OBJECT
public:
    ReplyAddMessageSender(SerialController* controller);

signals:
    void replyAddMessage(const QString&, const QString&, const QString&);
};

class ReplyAddMessage : public ReplyMessage
{
public:
    ReplyAddMessage(const QString& command, SerialController* controller);
    virtual bool isExpected(QList<RequestMessage>& msg) const override;

    virtual bool isValid(const QString& command) const override;
    virtual bool decode(const QString& command, QStringList& arguments) const override;
    virtual void process();

private:
    std::unique_ptr<ReplyAddMessageSender> m_sender;
};

#endif // ADDMESSAGE_H
