#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "serialcommunication.h"
#include "utils.h"

#include <QMainWindow>
#include <QLineEdit>
//#include <QTimer>
#include <memory>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool isFillupFiedls(QList<QLineEdit*> fields, const QString& errorMessage);
    bool checkAvailablePort(const QString& errorMessage);

private:
    //void readDictionary(const QString& filename);

private slots:
    void obtainWebsites();
    void addEntry();
    void retrieveEntry();
    void deleteEntry();
    void addEntryAndGenerate();
    void addWebsite(QString website, QString username);
    void deleteWebsite(QString website, QString username);
    void receiveReply(Utils::ReplyCode, QString message, QString status);
    void openPort();
    
private:
    Ui::MainWindow *ui;

    //QTimer availablePortTimer;
    std::unique_ptr<SerialCommunication, Utils::Deleter<SerialCommunication>> serialCommunication;

    QVector<QString> dictionar;

    void clearGUI();
};

#endif // MAINWINDOW_H
