#ifndef REQUESTMESSAGE_H
#define REQUESTMESSAGE_H

#include <QStringList>

class RequestMessage
{
public:
    RequestMessage(const QStringList& arguments);
    QString toString() const;
    QString getArgument(int index) const;

private:
    QStringList m_arguments;
};

#endif // REQUESTMESSAGE_H
