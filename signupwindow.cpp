#include "signupwindow.h"
#include "ui_signupwindow.h"
#include "serialcommunication.h"

#include <QCryptographicHash>
#include <QMessageBox>

SignUpWindow::SignUpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SignUpWindow),
    serialCommunication(SerialCommunication::getInstance())
{
    ui->setupUi(this);

    // set password mode for passwords fields
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
    ui->lineEditPassword2->setEchoMode(QLineEdit::Password);

    // set fix size
    this->setFixedSize(QSize(this->width() ,this->height()));

    // set window title
    this->setWindowTitle("SignUp");

    // connects
    connect(ui->buttonCreateUser, SIGNAL(clicked()), this, SLOT(createUserClicked()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    if( serialCommunication != nullptr )
    {
        connect(serialCommunication, SIGNAL(sendMessageToSingUp(QString,QString)), this, SLOT(displayMessage(QString,QString)));
    }
}

void SignUpWindow::createUserClicked()
{
    if ((ui->lineEditUsername->text().isEmpty()) || (ui->lineEditPassword->text().isEmpty()) || (ui->lineEditPassword2->text().isEmpty()))
    {
        //inform that the fields are not filled
        QMessageBox::information(NULL, "Information", "Not all the fields are filled up");
    }
    else
    {
        if (ui->lineEditPassword->text() != ui->lineEditPassword2->text())
        {
            // it means that he wrote different passwords
            QMessageBox::information(NULL, "Information", "The passwords are not matching");
        }
        else
        {
            // generates the new password
            QByteArray password = ui->lineEditPassword->text().toUtf8();
            QByteArray passwordSha1 = QCryptographicHash::hash(password, QCryptographicHash::Sha1);

            // create the new user
            SerialCommunication* serialCommunication = SerialCommunication::getInstance();
            if( serialCommunication->writeNewAccount(ui->lineEditUsername->text(), passwordSha1) )
            {
                // everything went fine & go back to login window
                QMessageBox::information(NULL, "Information", "New account is created");
                this->cancelClicked();
            }
            else
            {
                // some problems with the writting
                QMessageBox::information(NULL, "Information", "Something wrong when writting the new account");
            }
        }
    }
}

void SignUpWindow::cancelClicked()
{
    //clear all the text in the window
    ui->lineEditPassword->clear();
    ui->lineEditPassword2->clear();
    ui->lineEditUsername->clear();

    // we close the current window and emit a signal so the loginwindow will be shown
    this->close();
    emit closedWindow();
}

void SignUpWindow::displayMessage(QString message, QString status)
{
    QMessageBox::information(NULL, message, status);
}

SignUpWindow::~SignUpWindow()
{
    delete ui;
}
