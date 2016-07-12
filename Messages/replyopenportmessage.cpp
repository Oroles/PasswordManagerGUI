#include "replyopenportmessage.h"
#include "utils.h"

#include <exception>

ReplyOpenPortMessage::ReplyOpenPortMessage(const QString& command, SerialController* controller)
    : ReplyMessage(command),
      m_sender(std::unique_ptr<ReplyOpenPortMessageSender>(new ReplyOpenPortMessageSender(controller)))
{

}

bool ReplyOpenPortMessage::isExpected(QList<RequestMessage>& msgs) const
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

bool ReplyOpenPortMessage::isValid(const QString& command) const
{
    if (!command.isEmpty() && (Utils::ReplyCode)command[0].digitValue() == Utils::ReplyCode::ReplyOpenPort)
    {
        return true;
    }
    return false;
}

bool ReplyOpenPortMessage::decode(const QString& command, QStringList &arguments) const
{
    arguments = command.split(Utils::SEPARATOR);
    if (arguments.size() == 2)
    {
        return true;
    }
    return false;
}

void ReplyOpenPortMessage::process()
{
    if (isValid(m_command))
    {
        QStringList arguments;
        if (decode(m_command, arguments))
        {
            m_sender->replyOpenPortMessage(arguments[1]);
            return;
        }
    }
    throw std::exception();
}

ReplyOpenPortMessageSender::ReplyOpenPortMessageSender(SerialController *controller)
{
    connect(this, SIGNAL(replyOpenPortMessage(QString)), controller, SLOT(replyOpenPortMessage(QString)));
}

