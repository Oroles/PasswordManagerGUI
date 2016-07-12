#include "serialreceiver.h"

SerialReceiver::SerialReceiver(QObject *parent) :
   QObject(parent),
   m_isStop(false)
{
    moveToThread(&m_thread);
    connect(&m_thread, SIGNAL(started()), this, SLOT(process()));
}

void SerialReceiver::start()
{
    m_isStop = false;
    m_thread.start();
}

void SerialReceiver::stop()
{
    m_isStop = true;
    m_thread.wait();
}

void SerialReceiver::add(const QString &message)
{
    m_queue.add(message);
}

void SerialReceiver::process()
{
    while (!m_isStop)
    {
        QString msg;
        if (m_queue.wait_and_pop(msg))
        {
            emit receivedMessage(msg);
        }
    }
    QThread::currentThread()->quit();
}
