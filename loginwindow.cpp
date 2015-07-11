#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "serialcommunication.h"

#include <QCryptographicHash>
#include <QMessageBox>

LogInWindow::LogInWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogInWindow),
    serialCommunication(SerialCommunication::getInstance())
{
    ui->setupUi(this);

    // set password mode for password line edit
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);

    // make fix size the window
    this->setFixedSize(QSize(this->width(),this->height()));

    // set title
    this->setWindowTitle("LogIn");

    // connections
    connect(ui->pushButtonLogIn, SIGNAL(clicked()), this, SLOT(logInClicked()));
    connect(ui->pushButtonSignIn, SIGNAL(clicked()), this, SLOT(signInClicked()));

    if(serialCommunication != nullptr)
    {
        connect(serialCommunication, SIGNAL(sendMessageToLogin(QString,QString)), this, SLOT(displayMessage(QString,QString)));
    }
}

void LogInWindow::logInClicked()
{
    if ((ui->lineEditPassword->text().isEmpty()) || (ui->lineEditPassword->text().isEmpty()))
    {
        //inform that the fields are not filled
        QMessageBox::information(NULL, "Warrning", "Not all the fields are filled up");

    }
    else
    {
        // generate the Sha1 for password
        QByteArray byteArray = ui->lineEditPassword->text().toUtf8();
        QByteArray passwordSha1 = QCryptographicHash::hash(byteArray,QCryptographicHash::Sha1);

        // check the connection
        SerialCommunication* communication = SerialCommunication::getInstance();
        if ( communication != nullptr )
        {
            QMessageBox::information(NULL, "Information", "Something wrong with the port");
        }

        // check the password
        if (communication->checkPasswordAndUser(ui->lineEditUsername->text(), passwordSha1) == true)
        {
            // start the main window & clean the information
            ui->lineEditUsername->clear();
            ui->lineEditUsername->clear();

            emit startMainWindow();
            this->close();

        }
        else
        {
            QMessageBox::information(NULL, "Warrning", "Wrong password or username");
        }
    }
}

void LogInWindow::signInClicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Information");
    msgBox.setText("By creating another account will delete the previous one");
    msgBox.setInformativeText("Do you want to continue?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    switch( ret )
    {
        case QMessageBox::Ok:
        {
            // check the connection
            if ( serialCommunication != nullptr )
            {
                QMessageBox::information(NULL, "Information", "Something wrong with the port");
            }

            // clear the password and username from the board
            if ( serialCommunication->deletePasswordAndUser() )
            {
                // close the current window and signal other window to be active
                this->close();
                emit startSignUpWindow();
            }
        }
        case QMessageBox::Cancel:
        {
            //don't do anything
        }
    }

}

void LogInWindow::displayMessage(QString message, QString status)
{
    QMessageBox::information(NULL, message, status);
}

LogInWindow::~LogInWindow()
{
    delete ui;
}
