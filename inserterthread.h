#ifndef INSERTERTHREAD_H
#define INSERTERTHREAD_H

#include <QThread>
#include <QString>

class InserterThread : public QThread
{
    Q_OBJECT
public:
    InserterThread(QString password);
    void run() Q_DECL_OVERRIDE;

private:
    QString m_password;
};

#endif // INSERTERTHREAD_H
