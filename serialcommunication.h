#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QPair>

#include "utils.h"

/*
 *Sa creez doua pthread unu care sa apeleze metoda de read
 *iar celalalt sa apeleze method the write
 *write-ul citeste dintr-un buffer care este umplut de SerialPort
 *read-ul scrie intr-un buffer care este golit the catre SerialPort prin semnale
 *
 *sau folosesc semnalele de la qiodevice care is simple si trebuie sa fie bune
 *
 */

class SerialCommunication : public QObject
{
    Q_OBJECT
public:
    static SerialCommunication *getInstance();
    ~SerialCommunication();

    bool checkPasswordAndUser(const QString &username, const QString &password) const;
    bool writeNewAccount(const QString& username, const QString &password) const;
    bool deletePasswordAndUser() const;
    bool addEntry(const QString &website, const QString &username, const QString &password) const;
    bool retriveEntry(const QString &website, const QString &username) const;
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
