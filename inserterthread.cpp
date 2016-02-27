#include "inserterthread.h"

#include <QDebug>
#include <sstream>

InserterThread::InserterThread(QString password)
    : QThread(),
      m_password(password)
{

}

void InserterThread::run() {
    QThread::sleep(5);

    qDebug() << "Password retrived: " << m_password;
    for( QString::const_iterator it = m_password.constBegin(); it != m_password.constEnd(); ++it )
    {
        std::stringstream s;
        s << "xdotool key " << (*it).toLatin1();
        system(s.str().c_str());
    }
}
