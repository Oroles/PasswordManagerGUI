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


SOURCES += main.cpp\
        loginwindow.cpp \
    signupwindow.cpp \
    serialcommunication.cpp \
    mainwindow.cpp

HEADERS  += loginwindow.h \
    signupwindow.h \
    serialcommunication.h \
    mainwindow.h

FORMS    += loginwindow.ui \
    signupwindow.ui \
    mainwindow.ui
