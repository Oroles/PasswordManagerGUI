#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include "Controller/settings.h"

#include <QStringList>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QDateTime>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    serialController(nullptr),
    tableEntryModel(parent)

{
    ui->setupUi(this);

    this->setWindowTitle("Hlin");

    ui->buttonOpenPort->setStyleSheet("color: #333; border: 2px solid #555; border-radius: 11px; \
                                      padding: 5px; \
                                      background: qradialgradient(cx: 0.3, cy: -0.4, \
                                      fx: 0.3, fy: -0.4, \
                                      radius: 1.35, stop: 0 #fff, stop: 1 #888); \
                                      min-width: 80px");

    //ui->centralwidget->setStyleSheet("#tabWidget{background: green;}");
    connect(ui->checkBoxDisableAlert, SIGNAL(toggled(bool)), &Settings::getInstance(), SLOT(disableAlertMessages(bool)));
    connect(ui->checkBoxDisableLog, SIGNAL(toggled(bool)), &Settings::getInstance(), SLOT(disableLogMessages(bool)));
    connect(ui->checkBoxEnforceLength, SIGNAL(toggled(bool)), &Settings::getInstance(), SLOT(enforceMinimumLength(bool)));
    connect(ui->checkBoxSendIsAlive, SIGNAL(toggled(bool)), &Settings::getInstance(), SLOT(sendIsAlivePackage(bool)));

    // set table widget
    filterModel.setSourceModel(&tableEntryModel);
    ui->tableViewWebsites->setModel(&filterModel);
    ui->tableViewWebsites->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewWebsites->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableViewWebsites, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenuEvent(const QPoint&)));
    connect(ui->tableViewWebsites, &QTableView::clicked, [=](QModelIndex index) {
        ui->tableViewWebsites->selectRow(index.row());
    });

    connect(ui->lineEditSearch, &QLineEdit::textChanged, [=](const QString& filter) {
            filterModel.setFilterRegExp(filter);
        });

    // set password mode for password field
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);

    // set read only for text log
    ui->logTextEdit->setReadOnly(true);

    // connections
    connect(ui->buttonGetWebsites, SIGNAL(clicked()), this, SLOT(requestWebsites()));
    connect(ui->buttonAddEntry, SIGNAL(clicked()), this, SLOT(requestAddEntry()));
    connect(ui->buttonAddEntryAndGenerate, SIGNAL(clicked()), this, SLOT(requestAddEntryAndGenerate()));
    connect(ui->buttonOpenPort, SIGNAL(clicked(bool)), this, SLOT(requestOpenPort()));

    // set the spin box
    ui->spinBoxLength->setValue(Utils::MINIMUM_PASSWORD_LENGTH + 1);
    ui->spinBoxLength->setRange(Utils::MINIMUM_PASSWORD_LENGTH + 1, Utils::MAXIMUM_PASSWORD_LENGTH);

    // set the comboBox
    ui->comboBoxBautRate->addItems(QStringList({"9600", "14400", "19200"}));
    ui->comboBoxDatabits->addItems(QStringList({"6", "8", "10"}));
    ui->comboBoxDatabits->setCurrentIndex(1);
    ui->comboBoxParity->addItems(QStringList({"0", "1", "2"}));
    ui->comboBoxStopBits->addItems(QStringList({"0", "1", "2"}));
    ui->comboBoxStopBits->setCurrentIndex(1);
    ui->comboBoxFlowControl->addItems(QStringList({"0", "1"}));

    // set serial controller
    serialController = std::shared_ptr<SerialController>(new SerialController(this));
    connect(serialController.get(), SIGNAL(replyPortStatus(QString)), this, SLOT(replyPortStatus(QString)));
    connect(serialController.get(), SIGNAL(replyAddEntry(QString, QString)), this, SLOT(replyAddEntry(QString, QString)));
    connect(serialController.get(), SIGNAL(replyDeleteEntry(QString, QString)), this, SLOT(replyDeleteEntry(QString, QString)));
    connect(serialController.get(), SIGNAL(replyGetWebsite(QString,QString, long)), this, SLOT(replyGetWebsite(QString, QString, long)));
    connect(serialController.get(), SIGNAL(displayMessage(QString,QString,Qt::WindowModality)), this, SLOT(displayMessage(QString,QString,Qt::WindowModality)));
    connect(serialController.get(), SIGNAL(addToLog(QString)), this, SLOT(addToLog(QString)));

    dictionar = Utils::readDictionary("CommonWords", Utils::MINIMUM_PASSWORD_LENGTH, [](int a, int b){ return a < b; });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addWebsite(const TableEntry &entry)
{
    // add entry in the table
    tableEntryModel.append(entry);
}

void MainWindow::deleteWebsite(const TableEntry &entry)
{
    // remove entry
    tableEntryModel.deleteEntry(entry);
}

bool MainWindow::isFillupFiedls(QList<QLineEdit*> fields, const QString& errorMessage )
{
    for ( QLineEdit* field : fields ) {
        if (field->text().isEmpty()) {
            displayMessage("Information", errorMessage, Qt::ApplicationModal);
            addToLog(errorMessage);
            return false;
        }
    }
    return true;
}

bool MainWindow::checkAvailablePort(const QString& errorMessage) {
    if (!serialController->isPortOpened()) {
        displayMessage("Information", errorMessage, Qt::ApplicationModal);
        addToLog(errorMessage);
        return false;
    }
    return true;
}

void MainWindow::contextMenuEvent(const QPoint &pos)
{
    ui->tableViewWebsites->viewport()->mapToGlobal(pos);
    QModelIndex item = ui->tableViewWebsites->indexAt(pos);

    if (item.row() != -1)
    {
        //m_selectedRow = item.row();
        int row = item.row();
        ui->tableViewWebsites->selectRow(row);
        QMenu *menu=new QMenu(this);
        QAction* remove = menu->addAction("Remove");
        QAction* retrieve = menu->addAction("Request");
        QAction* desired = menu->exec(ui->tableViewWebsites->viewport()->mapToGlobal(pos));
        const TableEntry& info = tableEntryModel.getModel(row);
        if (desired == remove)
        {
            requestDeleteEntry(info.getWebsite(), info.getUsername());
        }
        if (desired == retrieve)
        {
            requestRetrieveEntry(info.getWebsite(), info.getUsername());
        }
    }
}

void MainWindow::requestAddEntry()
{
    // check if all the fields are not empty
    if (isFillupFiedls(QList<QLineEdit*>{ui->lineEditUsername, ui->lineEditPassword, ui->lineEditWebsite}, "Not all the fields are filled up"))
    {
        if (checkAvailablePort("Something wrong with the port"))
        {
            if ((!Settings::getInstance().IsEnforceMinimumLength()) || (ui->lineEditPassword->text().size() > Utils::MINIMUM_PASSWORD_LENGTH
                    && std::binary_search(dictionar.begin(), dictionar.end(), ui->lineEditPassword->text()) == false))
            {
                QString fullPassword = Utils::addPadding(ui->lineEditPassword->text());

                // add the password to the database via arduino
                if (!serialController->requestAddEntry(ui->lineEditWebsite->text(),
                                                ui->lineEditUsername->text(),
                                                fullPassword))
                {
                    // something went wrong
                    displayMessage("Information", "Could not add entry", Qt::ApplicationModal);
                    addToLog("Could not add entry");
                }
            }
            else
            {
                displayMessage("Information", "Problem with the password to short(min 8) or is common word", Qt::ApplicationModal);
                addToLog("Problem with the password to short(min 8) or is common word");
            }
        }
    }
    this->clearGUI();
}

void MainWindow::requestAddEntryAndGenerate()
{
    if (isFillupFiedls(QList<QLineEdit*>{ui->lineEditUsername, ui->lineEditWebsite}, "Not all the fields are filled up"))
    {
        if(!ui->checkBoxLetters->isChecked() && !ui->checkBoxNumbers->isChecked() && !ui->checkBoxSymbols->isChecked())
        {
            displayMessage("Information", "Not at leaset one checkbox is selected", Qt::ApplicationModal);
            addToLog("Not at least one checkbox is selected");
        }
        else
        {
            if (checkAvailablePort("Something wrong with the port"))
            {
                QString passwordLength = ui->spinBoxLength->text();
                QString allowedTypes = Utils::generateAllowTypes(ui->checkBoxSymbols->isChecked(), ui->checkBoxNumbers->isChecked(), ui->checkBoxLetters->isChecked());
                if (!serialController->requestAddEntryAndGenerate(ui->lineEditWebsite->text(),
                                                              ui->lineEditUsername->text(),
                                                              allowedTypes,
                                                              passwordLength))
                {
                    displayMessage("Information", "Could not add entry with password", Qt::ApplicationModal);
                    addToLog("Could not add entry");
                }
            }
        }
    }
    this->clearGUI();
}

void MainWindow::requestRetrieveEntry(const QString& website, const QString& username)
{
    if (checkAvailablePort("Something wrong with the port"))
    {
        // send the request to get the entry
        if (!serialController->requestRetrieveEntry(website, username))
        {
            // something went wrong
            displayMessage("Information", "Could not retrieve entry", Qt::ApplicationModal);
            addToLog("Could not retrieve entry");
        }
    }
}

void MainWindow::requestDeleteEntry(const QString& website, const QString& username)
{
    if (checkAvailablePort("Something wrong with the port"))
    {
        // send the request to delete the entry
        if (!serialController->requestDeleteEntry(website, username))
        {
            // something went wrong
            displayMessage("Information", "Could not delete entry", Qt::ApplicationModal);
            addToLog("Could not delete entry");
        }
    }
}

void MainWindow::requestWebsites()
{
    //clear the old entries
    tableEntryModel.clear();

    if (checkAvailablePort("Something wrong with the port"))
    {
        if( !serialController->requestWebsites() )
        {
            // something went wrong
            displayMessage("Information", "Couldn't obtain websites", Qt::ApplicationModal);
            addToLog("Could not obtain entry");
        }
    }
}

void MainWindow::requestOpenPort()
{
    if( ui->lineEditName->text().isEmpty() )
    {
        displayMessage("Information", "Not all the fields are filled up", Qt::ApplicationModal);
        addToLog("Not all the fields are filled up");
    }
    else
    {
        if (!serialController->openPort(ui->lineEditName->text(),
                                   ui->comboBoxBautRate->currentText().toInt(),
                                   ui->comboBoxDatabits->currentText().toInt(),
                                   ui->comboBoxParity->currentText().toInt(),
                                   ui->comboBoxStopBits->currentText().toInt(),
                                   ui->comboBoxFlowControl->currentText().toInt()))
        {
            displayMessage("Information", "Couldn't open the port", Qt::ApplicationModal);
            addToLog("Not all the fields are filled up");
        }
    }
}

void MainWindow::replyAddEntry(const QString& website, const QString& username)
{
    this->addWebsite( TableEntry( website, username, QDateTime::currentMSecsSinceEpoch() ));
}

void MainWindow::replyDeleteEntry(const QString& website, const QString& username)
{
    this->deleteWebsite(TableEntry(website, username, QDateTime::currentMSecsSinceEpoch())); //
}

void MainWindow::replyGetWebsite(const QString& website, const QString& username, long date)
{
    this->addWebsite(TableEntry(website, username, date));
}

void MainWindow::replyPortStatus(const QString &status)
{
    this->ui->labelPortStatus->setText(status);
}

void MainWindow::addToLog(const QString &msg)
{
    if (!Settings::getInstance().IsDisableLogMessages())
    {
        ui->logTextEdit->appendPlainText(msg);
    }
}

void MainWindow::displayMessage(const QString &title, const QString &msg, Qt::WindowModality modality)
{
    Utils::displayMessageBox(title, msg, modality, this);
}

/* Clears the text in the GUI */
void MainWindow::clearGUI()
{
    ui->lineEditWebsite->clear();
    ui->lineEditUsername->clear();
    ui->lineEditPassword->clear();
}
