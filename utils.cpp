#include "utils.h"

#include <QFile>
#include <QMessageBox>
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
            case 3:
                // set arg1 to Ok or Fail
                arg1 = arguments[1];
                return ReplyCode::ReplyDeleteEntry;
            case 4:
                // set arg1 to website and arg2 to username
                arg1 = arguments[1];
                arg2 = arguments[2];
                return ReplyCode::ReplyObtainWebsites;
            case 7:
                // set the message geneterated or failed
                arg1 = arguments[1];
                return ReplyCode::ReplyPasswordGenerated;
            case 5:
                // seth the message that is the correct port
                arg1 = arguments[1];
                return ReplyCode::ReplyCorrectPort;
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
                return arguments.size() == 3;
            case 3:
                return arguments.size() == 2;
            case 4:
                return arguments.size() == 3;
            case 5:
                return arguments.size() == 2;
            case 7:
                return true;
            default:
                return false;
        }
    }

    QString generateAllowTypes(bool allowSymbols, bool allowNumbers, bool allowLetters)
    {
        if ((allowSymbols == false) && (allowNumbers == false) && (allowLetters == true)) return "1";
        if ((allowSymbols == false) && (allowNumbers == true) && (allowLetters == false)) return "2";
        if ((allowSymbols == false) && (allowNumbers == true) && (allowLetters == true)) return "3";
        if ((allowSymbols == true) && (allowNumbers == false) && (allowLetters == false)) return "4";
        if ((allowSymbols == true) && (allowNumbers == false) && (allowLetters == true)) return "5";
        if ((allowSymbols == true) && (allowNumbers == true) && (allowLetters == false)) return "6";
        if ((allowSymbols == true) && (allowNumbers == true) && (allowLetters == true)) return "7";
        return "0";
    }

    QVector<QString> readDictionary(const QString &filename, int length, bool(*f)(int,int))
    {
        QVector<QString> dictionar;
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(NULL, "Information", "Couldn't open the database");
        }

        QTextStream in(&file);
        while (!in.atEnd()) {
            QString word = in.readLine();
            if (f(word.size(), length))
            {
                dictionar.push_back(word);
            }
        }
        std::sort(dictionar.begin(), dictionar.end());
        return dictionar;
    }
}
