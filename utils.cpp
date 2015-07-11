#include "utils.h"

#include <QStringList>
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

    QString encodeRequestCheckPasswordAndUser(const QString &username, const QString &password)
    {
        return "1\t" + username + "\t" + password + "\n";
    }

    QString encodeRequestWriteNewAccount(const QString &username, const QString &password)
    {
        return "2\t" + username + "\t" + password + "\n";
    }

    QString encodeRequestDeletePasswordAndUser()
    {
        return "3\t\n";
    }

    QString encodeRequestAddEntry(const QString& website, const QString& username, const QString& password)
    {
        return "4\t" + website + "\t" + username + "\t" + password + "\n";
    }

    QString encodeRequestRetriveEntry(const QString &website, const QString &username)
    {
        return "5\t" + website + "\t" + username + "\n";
    }

    QString encodeRequestDeleteEntry(const QString& website, const QString& username)
    {
        return "6\t" + website + "\t" + username + "\n";
    }

    QString encodeRequestObtainWebsites()
    {
        return "7\t\n";
    }

    ReplyCode decodeReply(QString reply, QString& arg1, QString& arg2)
    {
        QStringList arguments = reply.split("\t");
        switch(arguments[0].toInt())
        {
            case 1:
                // set arg1 to Ok or Fail
                arg1 = arguments[1];
                return ReplyCode::ReplyCheckPasswordAndUser;
            case 2:
                // set arg1 to Ok or Fail
                arg1 = arguments[1];
                return ReplyCode::ReplyWriteNewAccount;
            case 3:
                // set arg1 to Ok or Fail
                arg1 = arguments[1];
                return ReplyCode::ReplyDeletePasswordAndUser;
            case 4:
                // set arg1 to Ok or Fail
                arg1 = arguments[1];
                return ReplyCode::ReplyAddEntry;
            case 5:
                // set arg1 to the password
                arg1 = arguments[1];
                return ReplyCode::ReplyRetriveEntry;
            case 6:
                // set arg1 to Ok or Fail
                arg1 = arguments[1];
                return ReplyCode::ReplyDeleteEntry;
            case 7:
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
