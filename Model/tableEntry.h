#ifndef INFO_H
#define INFO_H

#include <QString>

class TableEntry
{
public:
    TableEntry(const QString& website, const QString& username, const long date);

    QString getWebsite() const;
    QString getUsername() const;
    QString getDate() const;

private:
    QString m_website;
    QString m_username;
    long m_date;
};

#endif // INFO_H
