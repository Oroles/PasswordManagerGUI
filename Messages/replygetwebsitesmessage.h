#ifndef REPLYGETWEBSITESMESSAGE_H
#define REPLYGETWEBSITESMESSAGE_H

#include "replymessage.h"
#include "Controller/serialcontroller.h"
#include <QObject>

//Private otherwise is nested in ReplyGetWebsiteMessage
class ReplyGetWebsiteMessageSender : public QObject
{
    Q_OBJECT
public:
    ReplyGetWebsiteMessageSender(SerialController* controller);

signals:
    void replyGetWebsitesMessage(const QString& website, const QString& username, long date);
};

class ReplyGetWebsitesMessage : public ReplyMessage
{
public:
    ReplyGetWebsitesMessage(const QString& command, SerialController* controller);
    virtual bool isExpected(QList<RequestMessage>&) const override;

    virtual bool isValid(const QString& command) const override;
    virtual bool decode(const QString& command, QStringList& arguments) const override;
    virtual void process();

private:
    std::unique_ptr<ReplyGetWebsiteMessageSender> m_sender;
};

#endif // REPLYGETWEBSITESMESSAGE_H
