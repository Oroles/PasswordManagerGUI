#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

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

signals:
    void startSignUpWindow();
    void startMainWindow();
    
private:
    Ui::LogInWindow *ui;
};

#endif // LOGINWINDOW_H
