#include "queryresultmodel.h"

QueryResultModel::QueryResultModel(QObject *parent) : QObject(parent)
{

}

bool QueryResultModel::error() const
{
    return this->m_error;
}
const QStringList& QueryResultModel::tableHeader() const
{
    return this->m_tableHeader;
}
const QList<QObject*> QueryResultModel::tableData() const
{
    return this->m_tableData;
}

void QueryResultModel::setError(bool error)
{
    this->m_error = error;
}
void QueryResultModel::setTableHeader(const QStringList& tableHeader)
{
    this->m_tableHeader = tableHeader;
}
void QueryResultModel::setTableData(const QObjectList& tableData)
{
    this->m_tableData = tableData;
}
