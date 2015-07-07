#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    serialCommunication(SerialCommunication::getInstance())
{
    ui->setupUi(this);

    //initialize table of websites
    QStringList tableWidgetHeader;
    tableWidgetHeader << "Website" << "Username";
    ui->tableWidgetWebsite->setHorizontalHeaderLabels(tableWidgetHeader);
    ui->tableWidgetWebsite->setColumnCount(2);

    // set password mode for password field
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);

    //connections
    connect(ui->buttonGetWebsites, SIGNAL(clicked()), this, SLOT(obtainWebsites()));
    connect(ui->buttonAddEntry, SIGNAL(clicked()), this, SLOT(addEntry()));
    connect(ui->buttonRetriveEntry, SIGNAL(clicked()), this, SLOT(retriveEntry()));
    connect(ui->buttonGeneratePassword, SIGNAL(clicked()), this, SLOT(generatePassword()));
    connect(ui->buttonDeleteEntry, SIGNAL(clicked()), this, SLOT(deleteEntry()));

    this->addWebsite("ana","maria");
    this->addWebsite("maria","ioana");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::obtainWebsites()
{
    // check if the connection is made
    if (serialCommunication == nullptr)
    {
        QMessageBox::information(NULL, "Information", "Something wrong with the port");
    }

    serialCommunication->obtainWebsites();
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
    if ((ui->lineEditUsername->text().isEmpty()) || (ui->lineEditPassword->text().isEmpty()) || (ui->lineEditWebsite->text().isEmpty()))
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
            // addd the password to the database via arduino
            if (serialCommunication->addEntry(ui->lineEditWebsite->text(),
                                              ui->lineEditUsername->text(),
                                              ui->lineEditPassword->text()))
            {
                // everything went fine then also add the entry to the available websites
                this->addWebsite(ui->lineEditPassword->text(), ui->lineEditPassword->text());
                QMessageBox::information(NULL, "Information", "New entry is added");
            }
            else
            {
                // something went wrong
                QMessageBox::information(NULL, "Information", "Could not add entry");
            }
        }
    }
}

void MainWindow::retriveEntry()
{
    if ((ui->lineEditUsername->text().isEmpty()) || (ui->lineEditWebsite->text().isEmpty()))
    {
        // not all information is filled
        QMessageBox::information(NULL, "Information", "Not all the fields are filled up");
    }
    else
    {
        //retrive entry

        // check if the connection is made
        if(serialCommunication == nullptr)
        {
            QMessageBox::information(NULL, "Information", "Something wrong with the port");
        }
        else
        {
            // send the request to get the entry
            if (serialCommunication->retriveEntry(ui->lineEditWebsite->text(),
                                                  ui->lineEditUsername->text()))
            {
                // everything went fine
                QMessageBox::information(NULL, "Information", "New entry is retrived");
            }
            else
            {
                // something went wrong
                QMessageBox::information(NULL, "Information", "Could not retrive entry");
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
            if (serialCommunication->deleteEntry(ui->lineEditWebsite->text(),
                                                 ui->lineEditUsername->text()))
            {
                // everything went fine
                this->deleteWebsite(ui->lineEditWebsite->text(), ui->lineEditUsername->text());
                QMessageBox::information(NULL, "Information", "Entry is deleted");
            }
            else
            {
                // something went wrong
                QMessageBox::information(NULL, "Information", "Could not delete entry");
            }
        }
    }
}

void MainWindow::generatePassword()
{
    // generate password

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
