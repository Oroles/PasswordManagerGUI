#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include <QMainWindow>

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

signals:
    void closedWindow();
    
private:
    Ui::SignUpWindow *ui;
};

#endif // SIGNUPWINDOW_H
