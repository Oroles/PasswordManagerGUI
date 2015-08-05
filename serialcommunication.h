#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QPair>

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
    static SerialCommunication *getInstance();
    ~SerialCommunication();

    bool addEntry(const QString &website, const QString &username, const QString &password, const QString& key) const;
    bool retrieveEntry(const QString &website, const QString &username, const QString &key) const;
    bool deleteEntry(const QString& website, const QString& username) const;
    bool obtainWebsites();

private:
    explicit SerialCommunication(QObject *parent = 0);

    SerialCommunication(SerialCommunication &rhs) = delete;
    SerialCommunication(SerialCommunication &&rhs) = delete;
    
signals:
    void sendMessageToLogin(QString msg, QString status);
    void sendMessageToSingUp(QString msg, QString status);
    void sendMessageToMain(Utils::ReplyCode, QString msg, QString status);
    void sendNewWebsite(QString website, QString username);
    void sendPassword(QString password);
    
private slots:
    void readBytes();
    void aboutToClose();
    

private:
    QSerialPort* serialPort;

    bool readConfiguration();
    QString m_portName;
    QSerialPort::BaudRate m_baudRate;
    QSerialPort::DataBits m_dataBits;
    QSerialPort::Parity m_parity;
    QSerialPort::StopBits m_stopBits;
    QSerialPort::FlowControl m_flowControl;
};

#endif // SERIALCOMMUNICATION_H
