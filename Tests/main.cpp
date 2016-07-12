#include <QtTest>
#include "testtableentry.h"
#include "testserialcommunication.h"
#include "testserialcontroller.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    TestTableEntry tableEntryTest;
    TestSerialCommunication serialCommunicationTest;
    TestSerialController serialControllerTest;
    return QTest::qExec(&tableEntryTest, argc, argv) |

            QTest::qExec(&serialCommunicationTest, argc, argv);/* |
            QTest::qExec(&serialControllerTest, argc, argv);*/
}
