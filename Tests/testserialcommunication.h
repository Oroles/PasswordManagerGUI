#ifndef TESTSERIALCOMMUNICATION_H
#define TESTSERIALCOMMUNICATION_H

#include <QObject>
#include <QTest>
#include <QString>

#include "Model/serialcommunication.h"

class TestSerialCommunication : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testCommunication();

private slots:
    void slotReceiveCommand(const QString& command);

private:
    SerialCommunication* m_serialCommunication;
    QString m_command;
};

#endif // TESTSERIALCOMMUNICATION_H
