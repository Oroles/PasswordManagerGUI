#include "serialcommunication.h"

SerialCommunication& SerialCommunication::getInstance()
{
    static SerialCommunication serialCommunication;
    return serialCommunication;
}

SerialCommunication::SerialCommunication(QObject *parent) :
    QObject(parent)
{
}

bool SerialCommunication::checkPasswordAndUser(QString username, QString password) const
{
    //TODO implement functionality
    return true;
}

bool SerialCommunication::writeNewAccount(QString username, QString password) const
{
    //TODO implement functionality
    return true;
}

bool SerialCommunication::deletePasswordAndUser() const
{
    //TODO implement functionality
    return true;
}
