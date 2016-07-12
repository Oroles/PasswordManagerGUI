#include "replygeneratedaddmessage.h"
#include "utils.h"

#include <exception>

ReplyGeneratedAddMessage::ReplyGeneratedAddMessage(const QString &command, SerialController *controller)
    : ReplyMessage(command)
    , m_sender(std::unique_ptr<ReplyGeneratedAddMessageSender>(new ReplyGeneratedAddMessageSender(controller)))
{

}

bool ReplyGeneratedAddMessage::isExpected(QList<RequestMessage>& msgs) const
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

bool ReplyGeneratedAddMessage::isValid(const QString& command) const
{
    if (!command.isEmpty() && (Utils::ReplyCode)command[0].digitValue() == Utils::ReplyCode::ReplyPasswordGenerated)
    {
        return true;
    }
    return false;
}

bool ReplyGeneratedAddMessage::decode(const QString& command, QStringList &arguments) const
{
    arguments = command.split(Utils::SEPARATOR);
    if (arguments.size() == 2)
    {
        return true;
    }
    return false;
}

void ReplyGeneratedAddMessage::process()
{
    if (isValid(m_command))
    {
        QStringList arguments;
        if (decode(m_command, arguments))
        {
            m_sender->replyGeneratedAddMessage(arguments[1]);
            return;
        }
    }
    throw std::exception();
}

ReplyGeneratedAddMessageSender::ReplyGeneratedAddMessageSender(SerialController *controller)
{
    connect(this, SIGNAL(replyGeneratedAddMessage(QString)), controller, SLOT(replyGeneratedAddMessage(QString)));
}
