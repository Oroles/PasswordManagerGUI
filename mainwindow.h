#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "serialcommunication.h"

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
    void retriveEntry();
    void deleteEntry();
    void generatePassword();
    void addWebsite(QString website, QString username);
    void deleteWebsite(QString website, QString username);
    
private:
    Ui::MainWindow *ui;
    SerialCommunication* serialCommunication;
};

#endif // MAINWINDOW_H
