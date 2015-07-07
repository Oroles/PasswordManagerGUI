#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QPair>

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

    bool checkPasswordAndUser(QString username, QString password) const;
    bool writeNewAccount(QString username, QString password) const;
    bool deletePasswordAndUser() const;
    bool addEntry(QString website, QString username, QString password) const;
    bool retriveEntry(QString website, QString username) const;
    bool deleteEntry(QString website, QString username) const;

    void obtainWebsites();

private:
    explicit SerialCommunication(QObject *parent = 0);

    SerialCommunication(SerialCommunication &rhs) = delete;
    SerialCommunication(SerialCommunication &&rhs) = delete;
    
signals:
    
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
