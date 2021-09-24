#ifndef TREEVIEWMODEL_H
#define TREEVIEWMODEL_H
//#include "dbworker.h"
#include <QObject>
#include "treeelement.h"

class DBWorker;

class TreeViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> data READ data  NOTIFY dataChanged)
public:
    explicit TreeViewModel(QObject *parent = nullptr);
    QList<QObject*> data();
    Q_INVOKABLE void log();
    Q_INVOKABLE void reset();
    void initModel(DBWorker &dbWorker);

    bool childrenAreChecked(const QString& parentElement);
    bool allChildrenAreChecked(const QString& parentElement);
    QStringList getCheckedChildren(const QString& parentElement);

    bool elementIsChecked(const QString& element);
    int elementCheckedState(const QString& element);
    void clear();


private:
    QList<QObject*> m_data;
    TreeElement* getTreeElement(const QString& text);

signals:
    void dataChanged();

public slots:
};

#endif // TREEVIEWMODEL_H
