#include "replymessage.h"

ReplyMessage::ReplyMessage(const QString &command) :
      m_command(command)
{

}

ReplyMessage::~ReplyMessage()
{

}

bool ReplyMessage::isExpected(QList<RequestMessage>& msg) const
{
    return false;
}
