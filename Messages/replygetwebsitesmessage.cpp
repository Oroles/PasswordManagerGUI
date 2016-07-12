#include "replygetwebsitesmessage.h"
#include "utils.h"

#include <exception>

ReplyGetWebsitesMessage::ReplyGetWebsitesMessage(const QString& command, SerialController *controller)
    : ReplyMessage(command)
    , m_sender(std::unique_ptr<ReplyGetWebsiteMessageSender>(new ReplyGetWebsiteMessageSender(controller)))
{
}

bool ReplyGetWebsitesMessage::isExpected(QList<RequestMessage>&) const
{
    return true;
}

bool ReplyGetWebsitesMessage::isValid(const QString& command) const
{
    if (!command.isEmpty() && (Utils::ReplyCode)command[0].digitValue() == Utils::ReplyCode::ReplyObtainWebsites)
    {
        return true;
    }
    return false;
}

bool ReplyGetWebsitesMessage::decode(const QString& command, QStringList &arguments) const
{
    arguments = command.split(Utils::SEPARATOR);
    if (arguments.size() == 4)
    {
        return true;
    }
    return false;
}

void ReplyGetWebsitesMessage::process()
{
    if (isValid(m_command))
    {
        QStringList arguments;
        if (decode(m_command, arguments))
        {
            m_sender->replyGetWebsitesMessage(arguments[1], arguments[2], arguments[3].toLong() );
            return;
        }
    }
    throw std::exception();
}

ReplyGetWebsiteMessageSender::ReplyGetWebsiteMessageSender(SerialController *controller)
{
    connect(this, SIGNAL(replyGetWebsitesMessage(QString, QString, long)), controller, SIGNAL(replyGetWebsite(QString, QString, long)));
}
