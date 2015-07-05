#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <QObject>

class SerialCommunication : public QObject
{
    Q_OBJECT
public:
    static SerialCommunication& getInstance();

    bool checkPasswordAndUser(QString username, QString password) const;
    bool writeNewAccount(QString username, QString password) const;
    bool deletePasswordAndUser() const;

private:
    explicit SerialCommunication(QObject *parent = 0);

    SerialCommunication(SerialCommunication &rhs) = delete;
    SerialCommunication(SerialCommunication &&rhs) = delete;
    
signals:
    
public slots:
    
};

#endif // SERIALCOMMUNICATION_H
