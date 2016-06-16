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
    static const int MINIMUM_PASSWORD_LENGTH = 7;
    static const int MAXIMUM_PASSWORD_LENGTH = 12;

    //These are all the request types
    enum class RequestTypes{ AddEntryRequest = 1,
                             RetriveEntryRequest = 2,
                             DeleteEntryRequest = 3,
                             ObtainWebsitesRequest = 4,
                             CheckCorrectPortRequest = 5,
                             GetHasValueRequest = 6,
                             AddAndGenerateEntryRequest = 7 };


    //These are the types of the replies it can receive from the Arduino
    //These have the local values, but there are also global values see function decode
    enum class ReplyCode {ReplyAddEntry,
                          ReplyDeleteEntry,
                          ReplyObtainWebsites,
                          ReplyPasswordGenerated,
                          ReplyCorrectPort,
                          ReplyError};

    static const QString ADD_ENTRY = "1";
    static const QString RETRIVE_ENTRY = "2";
    static const QString DELETE_ENTRY = "3";
    static const QString OBTAIN_WEBSITES = "4";
    static const QString ADD_GENERATE_ENTRY = "7";

    QString getName(QString field, const QList<QByteArray>& list);
    uint getValue(QString field, const QList<QByteArray>& list);

    ReplyCode decodeReply(QString reply, QString &arg1, QString &arg2);
    bool isValidCommand(const QString& command);
    QString generateAllowTypes(bool allowSymbols, bool allowNumbers, bool allowLetters);
    QVector<QString> readDictionary(const QString &filename, int length, bool (*f)(int, int));

    template < typename T>
    struct Deleter {
        void operator()(T* t) {
            delete t;
        }
    };

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
