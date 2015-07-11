#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include <QMainWindow>
#include "serialcommunication.h"

namespace Ui {
class SignUpWindow;
}

class SignUpWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SignUpWindow(QWidget *parent = 0);
    ~SignUpWindow();

private slots:
    void createUserClicked();
    void cancelClicked();
    void displayMessage(QString message, QString status);

signals:
    void closedWindow();
    
private:
    Ui::SignUpWindow *ui;
    SerialCommunication* serialCommunication;
};

#endif // SIGNUPWINDOW_H
