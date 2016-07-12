#ifndef REPLYDELETEMESSAGE_H
#define REPLYDELETEMESSAGE_H

#include "replymessage.h"
#include "Controller/serialcontroller.h"
#include <QObject>

//Private otherwie is nested in ReplyDeleteMessage
class ReplyDeleteMessageSender : public QObject
{
    Q_OBJECT
public:
    ReplyDeleteMessageSender(SerialController *controller);

signals:
    void replyDeleteMessage(const QString&, const QString&, const QString&);
};

class ReplyDeleteMessage : public ReplyMessage
{
public:
    ReplyDeleteMessage(const QString& command, SerialController *controller);
    virtual bool isExpected(QList<RequestMessage>& msg) const override;

    virtual bool isValid(const QString& command) const override;
    virtual bool decode(const QString& command, QStringList& arguments) const override;
    virtual void process();

private:
    std::unique_ptr<ReplyDeleteMessageSender> m_sender;
};

#endif // REPLYDELETEMESSAGE_H
