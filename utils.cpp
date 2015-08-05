#include "utils.h"

#include <QStringList>
#include <QString>
#include <QDebug>

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
        return "1" + SEPARATOR + website + SEPARATOR + username + SEPARATOR + password +  SEPARATOR + key + "\n";
    }

    QString encodeRequestRetrieveEntry(const QString &website, const QString &username, const QString& key)
    {
        return "2" + SEPARATOR + website + SEPARATOR + username + SEPARATOR + key + "\n";
    }

    QString encodeRequestDeleteEntry(const QString& website, const QString& username)
    {
        return "3" + SEPARATOR + website + SEPARATOR + username + "\n";
    }

    QString encodeRequestObtainWebsites()
    {
        return "4\n";
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
            default:
                qDebug() << "Something wrong with Utils::decodeReply";
                return ReplyCode::ReplyError;
        }
    }
}
