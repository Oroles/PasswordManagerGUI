#include "testserialcontroller.h"

#include "Tests/testhelper.h"

#include <QThread>

void TestSerialController::initTestCase()
{
    m_serialController = new SerialController(this);
    QVERIFY(m_serialController->openPort("/dev/pts/8", 9600, 8, 2, 0, 1));
    QThread::sleep(WAIT_REPLY_TYME);

    connect(m_serialController, SIGNAL(replyAddEntry(QString,QString)), this, SLOT(slotAddEntry(QString,QString)));
    connect(m_serialController, SIGNAL(replyDeleteEntry(QString,QString)), this, SLOT(slotDeleteEntry(QString,QString)));
    connect(m_serialController, SIGNAL(replyDeleteEntry(QString,QString)), this, SLOT(slotDeleteEntry(QString,QString)));
}

void TestSerialController::cleanupTestCase()
{
    delete m_serialController;
}

void TestSerialController::init()
{

}

void TestSerialController::cleanup()
{

}

void TestSerialController::testAddEntry()
{
    QVERIFY(m_serialController->addEntry("Website","Username", "12345678"));

    QThread::sleep(WAIT_REPLY_TYME);
    QCOMPARE(m_parameters.length(), 2);
    QCOMPARE(m_parameters[0], QString("Website"));
    QCOMPARE(m_parameters[1], QString("Username"));
    m_parameters.clear();
}

void TestSerialController::testGetEntry()
{
    QVERIFY(m_serialController->obtainWebsites());

    QThread::sleep(WAIT_REPLY_TYME);
    QCOMPARE(m_parameters.length(), 3);
    QCOMPARE(m_parameters[0], QString("Website"));
    QCOMPARE(m_parameters[1], QString("Username"));
    m_parameters.clear();
}

void TestSerialController::testDeleteEntry()
{
    QVERIFY(m_serialController->deleteEntry("Website", "Username"));

    QThread::sleep(WAIT_REPLY_TYME);
    QCOMPARE(m_parameters.length(), 2);
    QCOMPARE(m_parameters[0], QString("Website"));
    QCOMPARE(m_parameters[1], QString("Username"));
    m_parameters.clear();
}

void TestSerialController::slotAddEntry(const QString& website, const QString& username)
{
    m_parameters << website << username;
}

void TestSerialController::slotDeleteEntry(const QString& website, const QString& username)
{
    m_parameters << website << username;
}

void TestSerialController::slotGetWebsite(const QString& website, const QString& username, long date)
{
    m_parameters << website << username << QString::number(date);
}
