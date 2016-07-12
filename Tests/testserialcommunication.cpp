#include "testserialcommunication.h"
#include "Messages/requestmessage.h"
#include "testhelper.h"

#include <QThread>

void TestSerialCommunication::initTestCase()
{
    m_serialCommunication = new SerialCommunication(this, PORT_NAME, 9600, 8, 0, 1, 0);
    connect(m_serialCommunication, SIGNAL(receivedCommand(QString)), this, SLOT(slotReceiveCommand(QString)));
}

void TestSerialCommunication::cleanupTestCase()
{
    delete m_serialCommunication;
}

void TestSerialCommunication::init()
{

}

void TestSerialCommunication::cleanup()
{

}

void TestSerialCommunication::testCommunication()
{
    RequestMessage msg(QStringList{ QString("8")});
    m_serialCommunication->sendCommand(msg.toString().toStdString());

    QThread::sleep(WAIT_REPLY_TYME); //To make sure that we received the reply

    QCOMPARE(m_command, QString("8\rConnected"));
    m_command = "";
}

void TestSerialCommunication::slotReceiveCommand(const QString &command)
{
    m_command = command;
}
