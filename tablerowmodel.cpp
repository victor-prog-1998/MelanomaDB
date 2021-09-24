#include "tablerowmodel.h"

TableRowModel::TableRowModel(const QStringList &row, QObject *parent) : QObject(parent), m_row(row)
{

}

const QStringList &TableRowModel::row() const
{
    return m_row;
}
