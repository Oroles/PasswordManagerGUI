#include "replyisalivemessage.h"
#include "utils.h"

#include <exception>

ReplyIsAliveMessage::ReplyIsAliveMessage(const QString &command, SerialController *controller)
    : ReplyMessage(command)
    , m_sender(std::unique_ptr<ReplyIsAliveMessageSender>(new ReplyIsAliveMessageSender(controller)))
{

}

bool ReplyIsAliveMessage::isExpected(QList<RequestMessage>& msgs) const
{
    QStringList arguments;
    this->decode(m_command, arguments);

    //search for the command
    QList<RequestMessage>::iterator it = std::find_if(msgs.begin(), msgs.end(), [&arguments](const RequestMessage& msg) {
                                                        if (msg.getArgument(0) == arguments[0])
                                                        {
                                                            return true;
                                                        }
                                                        return false;
    });

    //remove all the types of this command
    if (it != msgs.end())
    {
        msgs.erase(std::remove_if(msgs.begin(), msgs.end(), [&arguments](const RequestMessage& msg){
                                                        if (msg.getArgument(0) == arguments[0])
                                                        {
                                                            return true;
                                                        }
                                                        return false; }));
        return true;
    }
    return false;
}

bool ReplyIsAliveMessage::isValid(const QString& command) const
{
    if (!command.isEmpty() && (Utils::ReplyCode)command[0].digitValue() == Utils::ReplyCode::ReplyIsAlive)
    {
        return true;
    }
    return false;
}

bool ReplyIsAliveMessage::decode(const QString& command, QStringList &arguments) const
{
    arguments = command.split(Utils::SEPARATOR);
    if (arguments.size() == 2)
    {
        return true;
    }
    return false;
}

void ReplyIsAliveMessage::process()
{
    if (isValid(m_command))
    {
        QStringList arguments;
        if (decode(m_command, arguments))
        {
            m_sender->replyIsAliveMessage(arguments[1]);
            return;
        }
    }
    throw std::exception();
}

ReplyIsAliveMessageSender::ReplyIsAliveMessageSender(SerialController *controller)
{
    connect(this, SIGNAL(replyIsAliveMessage(QString)), controller, SLOT(replyIsAliveMessage(QString)));
}
