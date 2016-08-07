#include "utils.h"

#include <QFile>
#include <QMessageBox>
#include <QStringList>
#include <QString>
#include <QDebug>

#include <algorithm>

#include "Controller/settings.h"

namespace Utils {

    QString getName(QString field, const QList<QByteArray>& list)
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

    uint getValue(QString field, const QList<QByteArray>& list)
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

    QString generateAllowTypes(bool allowSymbols, bool allowNumbers, bool allowLetters)
    {
        if ((allowSymbols == false) && (allowNumbers == false) && (allowLetters == true)) return "1";
        if ((allowSymbols == false) && (allowNumbers == true) && (allowLetters == false)) return "2";
        if ((allowSymbols == false) && (allowNumbers == true) && (allowLetters == true)) return "3";
        if ((allowSymbols == true) && (allowNumbers == false) && (allowLetters == false)) return "4";
        if ((allowSymbols == true) && (allowNumbers == false) && (allowLetters == true)) return "5";
        if ((allowSymbols == true) && (allowNumbers == true) && (allowLetters == false)) return "6";
        if ((allowSymbols == true) && (allowNumbers == true) && (allowLetters == true)) return "7";
        return "0";
    }

    QVector<QString> readDictionary(const QString &filename, int length, bool(*f)(int,int))
    {
        QVector<QString> dictionar;
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(NULL, "Information", "Couldn't open the database");
        }

        QTextStream in(&file);
        while (!in.atEnd()) {
            QString word = in.readLine();
            if (f(word.size(), length))
            {
                dictionar.push_back(word);
            }
        }
        std::sort(dictionar.begin(), dictionar.end());
        return dictionar;
    }

    void displayMessageBox(const QString& title, const QString& message, Qt::WindowModality modality, QWidget *parent)
    {
        if (!Settings::getInstance().IsDisableAlertMesssages())
        {
            QMessageBox* msg = new QMessageBox(parent);
            msg->setText(message);
            msg->setWindowTitle(title);
            msg->setWindowModality(modality);
            msg->show();
        }
    }
}
