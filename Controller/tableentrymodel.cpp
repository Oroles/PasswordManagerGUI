#include "tableentrymodel.h"

TableEntryModel::TableEntryModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

int TableEntryModel::rowCount(const QModelIndex &) const
{
    return m_data.count();
}

int TableEntryModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant TableEntryModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole)
    {
        return QVariant();
    }

    const TableEntry& info = m_data[index.row()];
    switch (index.column())
    {
        case 0: return info.getWebsite();
        case 1: return info.getUsername();
        case 2: return info.getDate();
        default: return QVariant();
    };
}

QVariant TableEntryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal)
    {
        return QVariant();
    }

    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    switch (section)
    {
        case 0: return "Website";
        case 1: return "Username";
        case 2: return "Date";
        default: return QVariant();
    }
 }

const TableEntry& TableEntryModel::getModel(int index) const
{
    return m_data[index];
}

void TableEntryModel::append(const TableEntry & info)
{
    beginInsertRows(QModelIndex(), m_data.count(), m_data.count());
    m_data.append(info);
    endInsertRows();
}

void TableEntryModel::deleteEntry(const TableEntry &info)
{
    int i = 0;
    QList<TableEntry>::iterator it = m_data.begin();
    while (it != m_data.end()) {
      if (it->getUsername() == info.getUsername() && it->getWebsite() == info.getWebsite())
      {
        beginRemoveColumns(QModelIndex(), i, i);
        it = m_data.erase(it);
        endRemoveRows();
      }
      else
      {
        ++it;
      }
      ++i;
    }
}

void TableEntryModel::clear()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

