#ifndef UTILS_H
#define UTILS_H

#include <QList>
#include <QByteArray>
#include <QString>

namespace Utils {
    static const QString SEPARATOR = ":";

    enum class ReplyCode {ReplyAddEntry,
                          ReplyRetrieveEntry,
                          ReplyDeleteEntry,
                          ReplyObtainWebsites,
                          CloseConnection,
                          ReplyError};

    QString getName(QString field, const QList<QByteArray>& list);
    uint getValue(QString field, const QList<QByteArray>& list);

    QString encodeRequestAddEntry(const QString& website, const QString& username, const QString& password, const QString &key);
    QString encodeRequestRetrieveEntry(const QString& website, const QString& username, const QString &key);
    QString encodeRequestDeleteEntry(const QString& website, const QString& username);
    QString encodeRequestObtainWebsites();
    QString encodeRequestCloseBluetoothConnection();

    ReplyCode decodeReply(QString reply, QString &arg1, QString &arg2);
    bool isValidCommand(const QString& command);
}

#endif // UTILS_H
