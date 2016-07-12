#ifndef INFOMODEL_H
#define INFOMODEL_H

#include <QAbstractTableModel>
#include "Model/tableEntry.h"

class TableEntryModel : public QAbstractTableModel
{
public:
    TableEntryModel(QObject *parent);

    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    const TableEntry& getModel(int index) const;

    void append(const TableEntry& info);
    void deleteEntry(const TableEntry& info);
    void clear();

private:
    QList<TableEntry> m_data;
};

#endif // INFOMODEL_H
