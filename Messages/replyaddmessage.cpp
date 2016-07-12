#include "replyaddmessage.h"
#include "utils.h"

#include <exception>

ReplyAddMessage::ReplyAddMessage(const QString &command, SerialController *controller)
    : ReplyMessage(command)
    , m_sender(std::unique_ptr<ReplyAddMessageSender>(new ReplyAddMessageSender(controller)))
{
}

bool ReplyAddMessage::isExpected(QList<RequestMessage>& msgs) const
{
    QStringList arguments;
    this->decode(m_command, arguments);

    //search for the command
    QList<RequestMessage>::iterator it = std::find_if(msgs.begin(), msgs.end(), [&arguments](const RequestMessage& msg) {
                                                        if ((msg.getArgument(0) == arguments[0]) &&
                                                            (msg.getArgument(1) == arguments[2]) &&
                                                            (msg.getArgument(2) == arguments[3]))
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

bool ReplyAddMessage::isValid(const QString& command) const
{
    if (!command.isEmpty() && (Utils::ReplyCode)command[0].digitValue() == Utils::ReplyCode::ReplyAddEntry)
    {
        return true;
    }
    return false;
}

bool ReplyAddMessage::decode(const QString& command, QStringList &arguments) const
{
    arguments = command.split(Utils::SEPARATOR);
    if (arguments.size() == 4)
    {
        return true;
    }
    return false;
}

void ReplyAddMessage::process()
{
    if (isValid(m_command))
    {
        QStringList arguments;
        if (decode(m_command, arguments))
        {
            m_sender->replyAddMessage(arguments[1], arguments[2], arguments[3]);
            return;
        }
    }
    throw std::exception();
}

ReplyAddMessageSender::ReplyAddMessageSender(SerialController *controller)
{
    connect(this, SIGNAL(replyAddMessage(QString, QString, QString)), controller, SLOT(replyAddMessage(QString, QString, QString)));
}

