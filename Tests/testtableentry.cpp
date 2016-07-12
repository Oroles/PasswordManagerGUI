#include "testtableentry.h"

#include "../Model/tableEntry.h"

void TestTableEntry::initTestCase()
{

}

void TestTableEntry::cleanupTestCase()
{

}

void TestTableEntry::init()
{

}

void TestTableEntry::cleanup()
{

}

void TestTableEntry::testGettersAndSetters()
{
    long time = QDateTime::currentMSecsSinceEpoch();
    TableEntry entry("website", "username", time);
    QCOMPARE(entry.getWebsite(), QString("website"));
    QCOMPARE(entry.getUsername(), QString("username"));

    QDateTime date;
    date.setMSecsSinceEpoch(time);
    QCOMPARE(entry.getDate(), date.toString());
}
