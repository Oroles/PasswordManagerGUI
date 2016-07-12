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

class SerialCommunication : public QObject
{
    Q_OBJECT
public:
    explicit SerialCommunication(QObject *parent, const QString name, int baudRate, int databits, int parity, int stopBits, int flowControl);
    ~SerialCommunication();

    SerialCommunication(SerialCommunication &rhs) = delete;
    SerialCommunication(SerialCommunication &&rhs) = delete;

    bool sendCommand(const std::string &command);
    void closeSerialPort();

signals:
    void receivedCommand(const QString& command);
    
private slots:
    void readBytes();

private:

    std::shared_ptr<QSerialPort> serialPort;
};

#endif // SERIALCOMMUNICATION_H
