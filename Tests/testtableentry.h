#ifndef TESTTABLEENTRY_H
#define TESTTABLEENTRY_H

#include <QObject>
#include <QtTest>

class TestTableEntry : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testGettersAndSetters();
};

#endif
