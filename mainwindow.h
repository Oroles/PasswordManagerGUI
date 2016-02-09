#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "serialcommunication.h"
#include "utils.h"

#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void obtainWebsites();
    void addEntry();
    void retrieveEntry();
    void deleteEntry();
    void addWebsite(QString website, QString username);
    void deleteWebsite(QString website, QString username);
    void receiveReply(Utils::ReplyCode, QString message, QString status);
    void displayPassword(QString status, QString password);
    
private:
    Ui::MainWindow *ui;
    SerialCommunication* serialCommunication;

    bool isCorrectSize(QString text) const;
    void clearGUI();
};

#endif // MAINWINDOW_H
