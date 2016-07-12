#include "tableEntry.h"

#include <QDateTime>

TableEntry::TableEntry(const QString &website, const QString &username, const long date)
    : m_website(website), m_username(username), m_date(date)
{

}

QString TableEntry::getWebsite() const
{
    return m_website;
}

QString TableEntry::getUsername() const
{
    return m_username;
}

QString TableEntry::getDate() const
{
    QDateTime date;
    date.setMSecsSinceEpoch(m_date);
    return date.toString();
}
