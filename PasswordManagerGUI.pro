#-------------------------------------------------
#
# Project created by QtCreator 2015-07-04T08:55:01
#
#-------------------------------------------------

QT       += core gui serialport
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PasswordManagerGUI
TEMPLATE = app

CONFIG += -static-runtime

QMAKE_CXXFLAGS_WARN_ON += -Wall

SOURCES += main.cpp\
    Model/serialcommunication.cpp \
    View/mainwindow.cpp \
    utils.cpp \
    Messages/replymessage.cpp \
    Messages/replyaddmessage.cpp \
    Messages/replydeletemessage.cpp \
    Messages/replygetwebsitesmessage.cpp \
    Messages/replyopenportmessage.cpp \
    Controller/serialcontroller.cpp \
    Messages/replygeneratedaddmessage.cpp \
    Messages/replyisalivemessage.cpp \
    Model/tableEntry.cpp \
    Controller/tableentrymodel.cpp \
    Messages/requestmessage.cpp \
    Controller/serialreceiver.cpp \
    Controller/settings.cpp


HEADERS  += \
    Model/serialcommunication.h \
    View/mainwindow.h \
    utils.h \
    Messages/replymessage.h \
    Messages/replyaddmessage.h \
    Messages/replydeletemessage.h \
    Messages/replygetwebsitesmessage.h \
    Messages/replyopenportmessage.h \
    Controller/serialcontroller.h \
    Messages/replygeneratedaddmessage.h \
    Messages/replyisalivemessage.h \
    Model/tableEntry.h \
    Controller/tableentrymodel.h \
    Messages/requestmessage.h \
    Model/syncqueue.hpp \
    Controller/serialreceiver.h \
    Controller/settings.h


FORMS    += \
    mainwindow.ui

test {
    message(Test build)
    QT += testlib
    TARGET = UnitTests

    SOURCES -= main.cpp

    HEADERS += Tests/testtableentry.h \
            Tests/testserialcommunication.h \
            Tests/testserialcontroller.h \
            Tests/testhelper.h

    SOURCES += Tests/main.cpp \
        Tests/testtableentry.cpp \
        Tests/testserialcommunication.cpp \
        Tests/testserialcontroller.cpp

} else {
    message(Normal build)

    HEADERS -= Tests/testtableentry.h

    SOURCES -= Tests/main.cpp \
        Tests/testtableentry.cpp
}
