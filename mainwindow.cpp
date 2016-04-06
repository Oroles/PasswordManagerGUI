#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QStringList>
#include <QDebug>
#include <QHeaderView>

#include "inserterthread.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    serialCommunication(SerialCommunication::getInstance())
{
    ui->setupUi(this);

    this->setWindowTitle("Password Managers");

    // set table widget
    ui->tableWidgetWebsite->setColumnCount(2);
    ui->tableWidgetWebsite->setHorizontalHeaderItem(0, new QTableWidgetItem("website"));
    ui->tableWidgetWebsite->setHorizontalHeaderItem(1, new QTableWidgetItem("username"));
    ui->tableWidgetWebsite->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // set password mode for password field
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);

    // set read only for text log
    ui->logTextEdit->setReadOnly(true);

    // connections
    connect(ui->buttonGetWebsites, SIGNAL(clicked()), this, SLOT(obtainWebsites()));
    connect(ui->buttonAddEntry, SIGNAL(clicked()), this, SLOT(addEntry()));
    connect(ui->buttonRetrieveEntry, SIGNAL(clicked()), this, SLOT(retrieveEntry()));
    connect(ui->buttonDeleteEntry, SIGNAL(clicked()), this, SLOT(deleteEntry()));
    connect(ui->buttonAddEntryAndGenerate, SIGNAL(clicked()), this, SLOT(addEntryAndGenerate()));

    if ( serialCommunication != nullptr )
    {
        // connect serialCommunication with GUI
        connect(serialCommunication, SIGNAL(sendMessageToMain(Utils::ReplyCode,QString,QString)), this, SLOT(receiveReply(Utils::ReplyCode,QString,QString)));
        connect(serialCommunication, SIGNAL(sendNewWebsite(QString,QString)), this, SLOT(addWebsite(QString,QString)));
        //connect(serialCommunication, SIGNAL(sendPassword(QString,QString)), this, SLOT(displayPassword(QString,QString)));
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
    if ((ui->lineEditUsername->text().isEmpty()) || (ui->lineEditPassword->text().isEmpty()) || (ui->lineEditWebsite->text().isEmpty()) )
    {
        // not all information is filled
        QMessageBox::information(NULL, "Information", "Not all the fields are filled up");
        ui->logTextEdit->appendPlainText("Not all the fields are filled up");
        this->clearGUI();
    }
    else
    {
        //add entry in the database

        // check if the connection is made
        if(serialCommunication == nullptr)
        {
            QMessageBox::information(NULL, "Information", "Something wrong with the port");
            ui->logTextEdit->appendPlainText("Something wrong with the port");
            this->clearGUI();
        }
        else
        {
            QString fullPassword = Utils::addPadding(ui->lineEditPassword->text());

            // add the password to the database via arduino
            if (!serialCommunication->addEntry(ui->lineEditWebsite->text(),
                                              ui->lineEditUsername->text(),
                                              fullPassword))
            {
                // something went wrong
                QMessageBox::information(NULL, "Information", "Could not add entry");
                ui->logTextEdit->appendPlainText("Could not add entry");
                this->clearGUI();
            }
        }
    }
}

QString generateAllowTypes(bool allowSymbols, bool allowNumbers, bool allowLetters)
{
    if ((allowSymbols == false) && (allowNumbers == false) && (allowLetters == true)) return "1";
    if ((allowSymbols == false) && (allowNumbers == true) && (allowLetters == false)) return "2";
    if ((allowSymbols == false) && (allowNumbers == true) && (allowLetters == true)) return "3";
    if ((allowSymbols == true) && (allowNumbers == false) && (allowLetters == false)) return "4";
    if ((allowSymbols == true) && (allowNumbers == false) && (allowLetters == true)) return "5";
    if ((allowSymbols == true) && (allowNumbers == true) && (allowLetters == false)) return "6";
    if ((allowSymbols == true) && (allowNumbers == true) && (allowLetters == true)) return "7";
    return "0";
}

void MainWindow::addEntryAndGenerate()
{
    if ((ui->lineEditUsername->text().isEmpty()) || (ui->lineEditWebsite->text().isEmpty()))
    {
        QMessageBox::information(NULL, "Information", "Not all the fields are filled up");
        ui->logTextEdit->appendPlainText("Not all the fields are filled up");
        this->clearGUI();
    }
    else
    {
        if(!ui->checkBoxLetters->isChecked() && !ui->checkBoxNumbers->isChecked() && !ui->checkBoxSymbols->isChecked())
        {
            QMessageBox::information(NULL, "Information", "Not at leaset one checkbox is selected");
            ui->logTextEdit->appendPlainText("Not at least one checkbox is selected");
        }
        else
        {
            if(serialCommunication == nullptr)
            {
                QMessageBox::information(NULL, "Information", "Something wrong with the port");
                ui->logTextEdit->appendPlainText("Something wrong with the port");
                this->clearGUI();
            }
            else
            {
                QString passwordLength = ui->spinBoxLength->text();
                QString allowedTypes = generateAllowTypes(ui->checkBoxSymbols->isChecked(), ui->checkBoxNumbers->isChecked(), ui->checkBoxLetters->isChecked());
                if (!serialCommunication->addEntryAndGenerate(ui->lineEditWebsite->text(),
                                                              ui->lineEditUsername->text(),
                                                              allowedTypes,
                                                              passwordLength))
                {
                    QMessageBox::information(NULL, "Information", "Could not add entry with password");
                    ui->logTextEdit->appendPlainText("Could not add entry");
                    this->clearGUI();
                }
            }
        }
    }
}

void MainWindow::retrieveEntry()
{
    if ((ui->lineEditUsername->text().isEmpty()) || (ui->lineEditWebsite->text().isEmpty()) )
    {
        // not all information is filled
        QMessageBox::information(NULL, "Information", "Not all the fields are filled up");
        ui->logTextEdit->appendPlainText("Not all the fields are filled up");
        this->clearGUI();
    }
    else
    {
        //retrieve entry

        // check if the connection is made
        if(serialCommunication == nullptr)
        {
            QMessageBox::information(NULL, "Information", "Something wrong with the port");
            ui->logTextEdit->appendPlainText("Something wrong with the port");
            this->clearGUI();
        }
        else
        {
            // send the request to get the entry
            if (!serialCommunication->retrieveEntry(ui->lineEditWebsite->text(),
                                                  ui->lineEditUsername->text()))
            {
                // something went wrong
                QMessageBox::information(NULL, "Information", "Could not retrieve entry");
                ui->logTextEdit->appendPlainText("Could not retrieve entry");
            }
            this->clearGUI();
        }
    }
}

void MainWindow::deleteEntry()
{
    if ((ui->lineEditUsername->text().isEmpty()) || (ui->lineEditWebsite->text().isEmpty()))
    {
        // not all information is filled
        QMessageBox::information(NULL, "Information", "Not all the fields are filled up");
        ui->logTextEdit->appendPlainText("Not all the fields are filled up");
        this->clearGUI();
    }
    else
    {
        // delete entry
        if (serialCommunication == nullptr)
        {
            QMessageBox::information(NULL, "Information", "Something wrong with the port");
            ui->logTextEdit->appendPlainText("Something wrong with the port");
            this->clearGUI();
        }
        else
        {
            // send the request to delete the entry
            if (!serialCommunication->deleteEntry(ui->lineEditWebsite->text(),
                                                 ui->lineEditUsername->text()))
            {
                // something went wrong
                QMessageBox::information(NULL, "Information", "Could not delete entry");
                ui->logTextEdit->appendPlainText("Could not delete entry");
                this->clearGUI();
            }
        }
    }
}

void MainWindow::obtainWebsites()
{
    //clear the old entries
    ui->tableWidgetWebsite->clearContents();
    ui->tableWidgetWebsite->setRowCount(0);

    // check if the connection is made
    if (serialCommunication == nullptr)
    {
        QMessageBox::information(NULL, "Information", "Something wrong with the port");
        ui->logTextEdit->appendPlainText("Not all the fields are filled up");
        this->clearGUI();
    }
    else
    {
        if( !serialCommunication->obtainWebsites() )
        {
            // something went wrong
            QMessageBox::information(NULL, "Information", "Couldn't obtain websites");
            ui->logTextEdit->appendPlainText("Could not obtain entry");
            this->clearGUI();
        }
    }
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
            ui->logTextEdit->appendPlainText("New entry is added");
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
            ui->logTextEdit->appendPlainText("Entry is deleted");
        }
        else
        {
            QMessageBox::information(NULL, message, status);
        }
        break;
    case Utils::ReplyCode::ReplyPasswordGenerated:
        QMessageBox::information(NULL, message, status);
        return;
    case Utils::ReplyCode::ReplyError:
        QMessageBox::information(NULL, message, status);
        break;
    default:
        break;
    }

    // clear the text in GUI.
    this->clearGUI();
}

/* Clears the text in the GUI */
void MainWindow::clearGUI()
{
    ui->lineEditWebsite->clear();
    ui->lineEditUsername->clear();
    ui->lineEditPassword->clear();
}
