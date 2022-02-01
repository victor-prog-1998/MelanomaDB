#ifndef TABLEROWMODEL_H
#define TABLEROWMODEL_H

#include <QObject>

class TableRowModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList row READ row NOTIFY rowChanged)
public:
    explicit TableRowModel(const QStringList& row, QObject *parent = nullptr);

    Q_INVOKABLE const QStringList& row() const;

signals:
    void rowChanged();

private:
    QStringList m_row;

public slots:
};

#endif // TABLEROWMODEL_H
