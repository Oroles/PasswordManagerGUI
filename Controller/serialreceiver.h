#ifndef SERIALRECEIVER_H
#define SERIALRECEIVER_H

#include <QObject>
#include <QThread>

#include "Model/syncqueue.hpp"

class SerialReceiver : public QObject
{
    Q_OBJECT
public:
    SerialReceiver(QObject *parent = NULL);

    void start();
    void stop();

public slots:
    void add(const QString& message);

signals:
    void receivedMessage(const QString& message);

private slots:
    void process();

private:
    QThread m_thread;
    bool m_isStop;
    SyncQueue<QString> m_queue;
};

#endif // SERIALRECEIVER_H
