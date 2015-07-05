#include "loginwindow.h"
#include "signupwindow.h"
#include "mainwindow.h"

#include <QApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LogInWindow loginWindow;
    SignUpWindow signUpWindow;
    MainWindow mainWindow;

    // make connections
    QObject::connect(&loginWindow, SIGNAL(startSignUpWindow()), &signUpWindow, SLOT(show()));
    QObject::connect(&loginWindow, SIGNAL(startMainWindow()), &mainWindow, SLOT(show()));
    QObject::connect(&signUpWindow, SIGNAL(closedWindow()), &loginWindow, SLOT(show()));

    loginWindow.show();
    
    return a.exec();
}
