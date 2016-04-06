#ifndef UTILS_H
#define UTILS_H

#include <QList>
#include <QByteArray>
#include <QString>

namespace Utils {
    static const QString SEPARATOR = "\r";
    static const QString PADDING = "\t";
    static const QString END_COMMAND = "\n";
    static const int KEY_SIZE = 16;
    static const int PASSWORD_SIZE = 16;

    enum class ReplyCode {ReplyAddEntry,
                          /*ReplyRetrieveEntry,*/
                          ReplyDeleteEntry,
                          ReplyObtainWebsites,
                          ReplyPasswordGenerated,
                          /*CloseConnection,*/
                          ReplyError};

    QString getName(QString field, const QList<QByteArray>& list);
    uint getValue(QString field, const QList<QByteArray>& list);

    ReplyCode decodeReply(QString reply, QString &arg1, QString &arg2);
    bool isValidCommand(const QString& command);

    template <typename T, int size = KEY_SIZE>
    T addPadding(const T& message) {
        if (message.size() % size == 0) {
            return message;
        }
        T result( message.size() + (size -( message.size() % size ) ), PADDING.at(0));
        std::copy_n(message.begin(), message.size(), result.begin());
        return result;
    }

    template <typename T, int size = KEY_SIZE>
    T removePadding(const T& message) {
        if (message.size() % size == 0) {
            return message;
        }
        T result = message.left( message.indexOf(PADDING.at(0)) );
        return result;
    }

    template <typename S, typename T>
    void generateRequest(S& s, T t) {
        s << t.toLatin1().constData();
        s << END_COMMAND.toLatin1().constData();
    }

    template <typename S, typename T, typename... Args>
    void generateRequest(S& s, T t, Args... args) {
        s << t.toLatin1().constData();
        s << SEPARATOR.toLatin1().constData();
        generateRequest(s, args...);
    }

}

#endif // UTILS_H
