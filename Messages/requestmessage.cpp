#include "requestmessage.h"

#include "utils.h"

RequestMessage::RequestMessage(const QStringList& arguments)
    : m_arguments(arguments)
{

}

QString RequestMessage::getArgument(int index) const
{
    return m_arguments[index];
}

QString RequestMessage::toString() const
{
    QString message;
    for ( int i = 0; i < m_arguments.size() - 1; ++i )
    {
        message += m_arguments[i];
        message += Utils::SEPARATOR;
    }
    message += m_arguments[m_arguments.size()-1];
    message += Utils::END_COMMAND;
    return message;
}
