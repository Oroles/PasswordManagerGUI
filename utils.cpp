#include "utils.h"

#include <QStringList>
#include <QDebug>

namespace Utils {
    QString getName(QString field, QList<QByteArray> list)
    {
        for( QString el : list )
        {
            if (el.contains(field))
            {
                QString value = el.split(" ")[1].simplified();
                qDebug() << "Field: " << field << " " << value;
                return value;
            }
        }
        throw std::exception();
        return "";
    }

    uint getValue(QString field, QList<QByteArray> list)
    {
        for( QString el : list )
        {
            if (el.contains(field))
            {
                uint value = (el.split(" ")[1]).toInt();
                qDebug() << "Field: " << field << " " << value;
                return value;
            }
        }
        throw std::exception();
        return 0;
    }
}
