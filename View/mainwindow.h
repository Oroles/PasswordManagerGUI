#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Controller/serialcontroller.h"
#include "Controller/tableentrymodel.h"

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QLineEdit>
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

public slots:
    void replyAddEntry(const QString &website, const QString &username);
    void replyDeleteEntry(const QString &website, const QString &username);
    void replyGetWebsite(const QString& website, const QString& username, long date);
    void replyPortStatus(const QString& status);

    void addToLog(const QString& msg);
    void displayMessage(const QString& title, const QString& msg, Qt::WindowModality modality);
    void contextMenuEvent(const QPoint& pos);

private slots:
    void requestWebsites();
    void requestAddEntry();
    void requestAddEntryAndGenerate();
    void requestOpenPort();

private:
    void deleteWebsite(const TableEntry& entry);
    void addWebsite(const TableEntry& entry);
    void clearGUI();

    void requestDeleteEntry(const QString& website, const QString& username);
    void requestRetrieveEntry(const QString& website, const QString& username);
    bool checkAvailablePort(const QString& errorMessage);

    bool isFillupFiedls(QList<QLineEdit*> fields, const QString& errorMessage);

private:
    Ui::MainWindow *ui;

    std::shared_ptr<SerialController> serialController;

    TableEntryModel tableEntryModel;
    QSortFilterProxyModel filterModel;

    QVector<QString> dictionar;
};

#endif // MAINWINDOW_H
