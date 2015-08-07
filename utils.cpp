#include "utils.h"

#include <QStringList>
#include <QString>
#include <QDebug>

#include <algorithm>

namespace Utils {

    QString getName(QString field, const QList<QByteArray>& list)
    {
        for( QString el : list )
        {
            if (el.contains(field))
            {
                QString value = el.split(" ")[1].simplified();
                qDebug() << "Field: " << field << " " << value;
                return value;
            }
        }
        throw std::exception();
        return "";
    }

    uint getValue(QString field, const QList<QByteArray>& list)
    {
        for( QString el : list )
        {
            if (el.contains(field))
            {
                uint value = (el.split(" ")[1]).toInt();
                qDebug() << "Field: " << field << " " << value;
                return value;
            }
        }
        throw std::exception();
        return 0;
    }

    QString encodeRequestAddEntry(const QString& website, const QString& username, const QString& password, const QString& key)
    {
        return "1" + SEPARATOR + website +
                     SEPARATOR + username +
                     SEPARATOR + password +
                     SEPARATOR + key + END_COMMAND;
    }

    QString encodeRequestRetrieveEntry(const QString &website, const QString &username, const QString& key)
    {
        return "2" + SEPARATOR + website +
                     SEPARATOR + username +
                     SEPARATOR + key + END_COMMAND;
    }

    QString encodeRequestDeleteEntry(const QString& website, const QString& username)
    {
        return "3" + SEPARATOR + website +
                     SEPARATOR + username + END_COMMAND;
    }

    QString encodeRequestObtainWebsites()
    {
        return "4" + END_COMMAND;
    }

    QString encodeRequestCloseBluetoothConnection()
    {
        return "5" + END_COMMAND;
    }

    ReplyCode decodeReply(QString reply, QString& arg1, QString& arg2)
    {
        QStringList arguments = reply.split(SEPARATOR);
        switch(arguments[0].toInt())
        {
            case 1:
                // set arg1 to Ok or Fail
                arg1 = arguments[1];
                return ReplyCode::ReplyAddEntry;
            case 2:
                // set arg1 to the password
                arg1 = arguments[1];
                return ReplyCode::ReplyRetrieveEntry;
            case 3:
                // set arg1 to Ok or Fail
                arg1 = arguments[1];
                return ReplyCode::ReplyDeleteEntry;
            case 4:
                // set arg1 to website and arg2 to username
                arg1 = arguments[1];
                arg2 = arguments[2];
                return ReplyCode::ReplyObtainWebsites;
            case 5:
                return ReplyCode::CloseConnection;
            default:
                qDebug() << "Something wrong with Utils::decodeReply";
                return ReplyCode::ReplyError;
        }
    }

    // checks if there is the right number of arguments
    bool isValidCommand(const QString& command) {
        QStringList arguments = command.split(SEPARATOR);
        switch(arguments[0].toInt())
        {
            case 1:
                return arguments.size() == 2;
            case 2:
                return arguments.size() == 2;
            case 3:
                return arguments.size() == 2;
            case 4:
                return arguments.size() == 3;
            case 5:
                return arguments.size() == 2;
            default:
                return false;
        }
    }

    QString addPadding(const QString& message) {
        if (message.size() % KEY_SIZE == 0) {
            return message;
        }
        QString result( message.size() + (KEY_SIZE -( message.size() % KEY_SIZE ) ), PADDING.at(0));
        std::copy_n(message.begin(), message.size(), result.begin());
        return result;
    }

    QString removePadding(const QString& message) {
        if (message.size() % KEY_SIZE == 0) {
            return message;
        }
        QString result = message.left( message.indexOf(PADDING.at(0)) );
        return result;
    }
}
