#ifndef UTILS_H
#define UTILS_H

#include <QList>
#include <QByteArray>
#include <QString>

namespace Utils {
    enum class ReplyCode {ReplyCheckPasswordAndUser,
                          ReplyWriteNewAccount,
                          ReplyDeletePasswordAndUser,
                          ReplyAddEntry,
                          ReplyRetriveEntry,
                          ReplyDeleteEntry,
                          ReplyObtainWebsites,
                          ReplyError};

    QString getName(QString field, const QList<QByteArray>& list);
    uint getValue(QString field, const QList<QByteArray>& list);

    QString encodeRequestCheckPasswordAndUser(const QString& username, const QString& password);
    QString encodeRequestWriteNewAccount(const QString& username, const QString& password);
    QString encodeRequestDeletePasswordAndUser();
    QString encodeRequestAddEntry(const QString& website, const QString& username, const QString& password);
    QString encodeRequestRetriveEntry(const QString& website, const QString& username);
    QString encodeRequestDeleteEntry(const QString& website, const QString& username);
    QString encodeRequestObtainWebsites();

    ReplyCode decodeReply(QString reply, QString &arg1, QString &arg2);
}

#endif // UTILS_H
