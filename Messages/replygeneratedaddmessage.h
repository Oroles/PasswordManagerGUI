#ifndef REPLYGENERATEDADDMESSAGE_H
#define REPLYGENERATEDADDMESSAGE_H

#include "replymessage.h"
#include "Controller/serialcontroller.h"
#include <QObject>

//Private otherwise is nested in ReplyGeneratedAddMessage
class ReplyGeneratedAddMessageSender : public QObject
{
    Q_OBJECT
public:
    ReplyGeneratedAddMessageSender(SerialController* controller);

signals:
    void replyGeneratedAddMessage(const QString&);
};

class ReplyGeneratedAddMessage : public ReplyMessage
{
public:
    ReplyGeneratedAddMessage(const QString& command, SerialController* controller);
    virtual bool isExpected(QList<RequestMessage>& msg) const override;

    virtual bool isValid(const QString& command) const override;
    virtual bool decode(const QString& command, QStringList& arguments) const override;
    virtual void process();

private:
    std::unique_ptr<ReplyGeneratedAddMessageSender> m_sender;
};

#endif // REPLYGENERATEDADDMESSAGE_H
