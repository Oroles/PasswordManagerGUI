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
}
