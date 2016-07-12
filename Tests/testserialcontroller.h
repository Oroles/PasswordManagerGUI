#ifndef TESTSERIALCONTROLLER_H
#define TESTSERIALCONTROLLER_H

#include <QObject>
#include <QTest>

#include "Controller/serialcontroller.h"

class TestSerialController : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testAddEntry();
    void testGetEntry();
    void testDeleteEntry();

private slots:
    void slotAddEntry(const QString& website, const QString& username);
    void slotDeleteEntry(const QString& website, const QString& username);
    void slotGetWebsite(const QString& website, const QString& username, long date);

private:
    SerialController* m_serialController;
    QStringList m_parameters;
};

#endif // TESTSERIALCONTROLLER_H
