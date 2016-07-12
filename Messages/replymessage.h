#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include "requestmessage.h"

class ReplyMessage
{
public:
    explicit ReplyMessage(const QString& command);
    virtual ~ReplyMessage();

    virtual bool isExpected(QList<RequestMessage>& msg) const;

    virtual bool isValid(const QString& command) const = 0;
    virtual bool decode(const QString& command, QStringList& arguments) const = 0;
    virtual void process() = 0;

signals:


public slots:

protected:
    QString m_command;

};

#endif // MESSAGE_H
