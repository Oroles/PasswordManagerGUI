#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QPair>
#include <QTimer>
#include <memory>

#include "utils.h"

/*
 * Compress the string that I send, the compress should be one to one,
 * otherwise for two websites there will one entry in database
 * other solution is rangeencoding
 */

class SerialCommunication : public QObject
{
    Q_OBJECT
public:
    ~SerialCommunication();

    bool addEntry(const QString &website, const QString &username, const QString &password) const;
    bool addEntryAndGenerate(const QString &website, const QString &username, const QString &types, const QString &passwordLength) const;
    bool retrieveEntry(const QString &website, const QString &username) const;
    bool deleteEntry(const QString& website, const QString& username) const;
    bool obtainWebsites();
    bool checkCorrectPort();
    bool isPortOpened();

    explicit SerialCommunication(QObject *parent, const QString name, int baudRate, int databits, int parity, int stopBits, int flowControl);

    SerialCommunication(SerialCommunication &rhs) = delete;
    SerialCommunication(SerialCommunication &&rhs) = delete;

    void closeSerialPort();
    
signals:
    void sendMessageToMain(Utils::ReplyCode, QString msg, QString status);
    void sendNewWebsite(QString website, QString username);
    
private slots:
    void readBytes();

private:
    bool m_isPortOpened;
    QTimer m_openPortTimer;

    std::shared_ptr<QSerialPort> serialPort;
};

#endif // SERIALCOMMUNICATION_H
