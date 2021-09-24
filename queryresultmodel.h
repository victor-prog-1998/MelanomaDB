#ifndef QUERYRESULTMODEL_H
#define QUERYRESULTMODEL_H

#include <QObject>

class QueryResultModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool error READ error NOTIFY propertyChanged)
    Q_PROPERTY(QStringList tableHeader READ tableHeader NOTIFY propertyChanged)
    Q_PROPERTY(QList<QObject*> tableData READ tableData NOTIFY propertyChanged)



public:
    explicit QueryResultModel(QObject *parent = nullptr);


    bool error() const;
    const QStringList& tableHeader() const;
    const QList<QObject*> tableData() const;

    void setError(bool error);
    void setTableHeader(const QStringList& tableHeader);
    void setTableData(const QObjectList& tableData);

private:
    bool m_error = false;
    QStringList m_tableHeader;
    QObjectList m_tableData;



signals:

    void propertyChanged();

public slots:
};

#endif // QUERYRESULTMODEL_H
