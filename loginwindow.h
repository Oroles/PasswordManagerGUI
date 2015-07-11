#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "serialcommunication.h"

namespace Ui {
class LogInWindow;
}

class LogInWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit LogInWindow(QWidget *parent = 0);
    ~LogInWindow();

private slots:
    void logInClicked();
    void signInClicked();
    void displayMessage(QString message, QString status);

signals:
    void startSignUpWindow();
    void startMainWindow();
    
private:
    Ui::LogInWindow *ui;
    SerialCommunication* serialCommunication;
};

#endif // LOGINWINDOW_H
