#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QStringList>
#include <QThread>
#include <sstream>
#include <QDebug>
#include <QProcess>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    serialCommunication(SerialCommunication::getInstance())
{
    ui->setupUi(this);

    // set table widget
    ui->tableWidgetWebsite->setColumnCount(2);
    ui->tableWidgetWebsite->setHorizontalHeaderItem(0, new QTableWidgetItem("website"));
    ui->tableWidgetWebsite->setHorizontalHeaderItem(1, new QTableWidgetItem("username"));
    ui->tableWidgetWebsite->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // set password mode for password field
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);

    // connections
    connect(ui->buttonGetWebsites, SIGNAL(clicked()), this, SLOT(obtainWebsites()));
    connect(ui->buttonAddEntry, SIGNAL(clicked()), this, SLOT(addEntry()));
    connect(ui->buttonRetrieveEntry, SIGNAL(clicked()), this, SLOT(retrieveEntry()));
    connect(ui->buttonGeneratePassword, SIGNAL(clicked()), this, SLOT(generatePassword()));
    connect(ui->buttonDeleteEntry, SIGNAL(clicked()), this, SLOT(deleteEntry()));

    if ( serialCommunication != nullptr )
    {
        // connect serialCommunication with GUI
        connect(serialCommunication, SIGNAL(sendMessageToMain(Utils::ReplyCode,QString,QString)), this, SLOT(receiveReply(Utils::ReplyCode,QString,QString)));
        connect(serialCommunication, SIGNAL(sendNewWebsite(QString,QString)), this, SLOT(addWebsite(QString,QString)));
        connect(serialCommunication, SIGNAL(sendPassword(QString)), this, SLOT(displayPassword(QString)));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addWebsite(QString website, QString username)
{
    // add entry in the table
    ui->tableWidgetWebsite->insertRow( 0 );
    ui->tableWidgetWebsite->setItem(0, 0, new QTableWidgetItem(website));
    ui->tableWidgetWebsite->setItem(0, 1, new QTableWidgetItem(username));
}

void MainWindow::deleteWebsite(QString website, QString username)
{
    //iterate over rows and when we find the wanted one, remove the row and break;
    for(int i = 0; i < ui->tableWidgetWebsite->rowCount(); ++i)
    {
        if ((ui->tableWidgetWebsite->item(i,0)->text() == website) && (ui->tableWidgetWebsite->item(i,1)->text() == username))
        {
            // delete the row we found
            ui->tableWidgetWebsite->removeRow(i);

            // we don't have to search anymore, there should be only one row
            break;
        }
    }
}

void MainWindow::addEntry()
{
    // check if all the fields are not empty
    if ((ui->lineEditUsername->text().isEmpty()) || (ui->lineEditPassword->text().isEmpty()) ||
            (ui->lineEditWebsite->text().isEmpty()) || (ui->lineEditKey->text().isEmpty()) )
    {
        // not all information is filled
        QMessageBox::information(NULL, "Information", "Not all the fields are filled up");
    }
    else
    {
        //add entry in the database

        // check if the connection is made
        if(serialCommunication == nullptr)
        {
            QMessageBox::information(NULL, "Information", "Something wrong with the port");
        }
        else
        {
            QString fullPassword = Utils::addPadding(ui->lineEditPassword->text());
            QString fullKey = Utils::addPadding(ui->lineEditKey->text());

            // add the password to the database via arduino
            if (!serialCommunication->addEntry(ui->lineEditWebsite->text(),
                                              ui->lineEditUsername->text(),
                                              fullPassword,
                                              fullKey))
            {
                // something went wrong
                QMessageBox::information(NULL, "Information", "Could not add entry");
            }
        }
    }
}

void MainWindow::retrieveEntry()
{
    if ((ui->lineEditUsername->text().isEmpty()) || (ui->lineEditWebsite->text().isEmpty()) || (ui->lineEditKey->text().isEmpty()))
    {
        // not all information is filled
        QMessageBox::information(NULL, "Information", "Not all the fields are filled up");
    }
    else
    {
        //retrieve entry

        // check if the connection is made
        if(serialCommunication == nullptr)
        {
            QMessageBox::information(NULL, "Information", "Something wrong with the port");
        }
        else
        {
            // send the request to get the entry
            if (!serialCommunication->retrieveEntry(ui->lineEditWebsite->text(),
                                                  ui->lineEditUsername->text(),
                                                  ui->lineEditKey->text()))
            {
                // something went wrong
                QMessageBox::information(NULL, "Information", "Could not retrieve entry");
            }
        }
    }
}

void MainWindow::deleteEntry()
{
    if ((ui->lineEditUsername->text().isEmpty()) || (ui->lineEditWebsite->text().isEmpty()))
    {
        // not all information is filled
        QMessageBox::information(NULL, "Information", "Not all the fields are filled up");
    }
    else
    {
        // delete entry
        if (serialCommunication == nullptr)
        {
            QMessageBox::information(NULL, "Information", "Something wrong with the port");
        }
        else
        {
            // send the request to delete the entry
            if (!serialCommunication->deleteEntry(ui->lineEditWebsite->text(),
                                                 ui->lineEditUsername->text()))
            {
                // something went wrong
                QMessageBox::information(NULL, "Information", "Could not delete entry");
            }
        }
    }
}

void MainWindow::obtainWebsites()
{
    //clear the old entries
    ui->tableWidgetWebsite->clear();
    ui->tableWidgetWebsite->setRowCount(0);

    // check if the connection is made
    if (serialCommunication == nullptr)
    {
        QMessageBox::information(NULL, "Information", "Something wrong with the port");
    }
    else
    {
        if( !serialCommunication->obtainWebsites() )
        {
            // something went wrong
            QMessageBox::information(NULL, "Information", "Couldn't obtain websites");
        }
    }
}

/* Generates passwords */
void MainWindow::generatePassword()
{

    // password size
    static const int PASSWORD_SIZE = 16;

    QString password;
    for( int i = 0; i < PASSWORD_SIZE; ++i)
    {
        password.append(QChar(97+qrand()*(122-97)));//generates random small letters
    }

    // assign the password in the password field
    ui->lineEditPassword->setText(password);
}

void MainWindow::receiveReply(Utils::ReplyCode reply, QString message, QString status)
{
    switch (reply) {
    case Utils::ReplyCode::ReplyAddEntry:
        if( status == "OK" )
        {
            // everything went fine then also add the entry to the available websites
            this->addWebsite(ui->lineEditWebsite->text(), ui->lineEditUsername->text());
            QMessageBox::information(NULL, "Information", "New entry is added");
        }
        else
        {
            QMessageBox::information(NULL, message, status);
        }

        break;
    case Utils::ReplyCode::ReplyDeleteEntry:
        if( status == "OK" )
        {
            // everything went fine
            this->deleteWebsite(ui->lineEditWebsite->text(), ui->lineEditUsername->text());
            QMessageBox::information(NULL, "Information", "Entry is deleted");
        }
        else
        {
            QMessageBox::information(NULL, message, status);
        }
        break;
    case Utils::ReplyCode::ReplyError:
        QMessageBox::information(NULL, message, status);
        break;
    default:
        break;
    }

    // clear the text in GUI.
    this->clearGUI();
}

/* Check if the size of the string is 16, needed for encryption */

/* Should not be used, if password is longer or shorter than 16, they should be adapted
 * to 16. If the password is longer, still not OK, if is shorter, put also the password
 * size in password and padding it with some characters until is 18 long( 2 - length +
 * 16 - password)
 * then also the length should be store in database, when the decryption is used, to
 * remove the extra paddings*/
bool MainWindow::isCorrectSize(QString text) const
{
    return text.size() == Utils::KEY_SIZE;
}

/* Clears the text in the GUI */
void MainWindow::clearGUI()
{
    ui->lineEditWebsite->clear();
    ui->lineEditUsername->clear();
    ui->lineEditPassword->clear();
    ui->lineEditKey->clear();
}

/*
 *It's safe because I can not execute invalid passwords
 *If someone tries to execute a malicous command, by setting the password
 *I split it in letters and execute each letter separtly therefore,
 *the malicous command is split.
 */
void MainWindow::displayPassword(QString password)
{
    this->clearGUI();
    password = Utils::removePadding(password);

    QThread::sleep(5);

    qDebug() << "Password retrived: " << password;
    for( QString::const_iterator it = password.constBegin(); it != password.constEnd(); ++it )
    {
        std::stringstream s;
        s << "xdotool key " << (*it).toLatin1();
        system(s.str().c_str());

        /*QProcess process;
        process.startDetached(QString("xdotool key ").append((*it).toLatin1()) );
        process.waitForStarted(3000);*/
    }
}
