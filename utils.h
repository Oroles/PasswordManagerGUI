#ifndef UTILS_H
#define UTILS_H

#include <QList>
#include <QByteArray>
#include <QString>

namespace Utils {
    QString getName(QString field, QList<QByteArray> list);
    uint getValue(QString field, QList<QByteArray> list);
}

#endif // UTILS_H
