#include "treeviewmodel.h"
#include "dbworker.h"
#include <QDebug>

TreeViewModel::TreeViewModel(QObject *parent) : QObject(parent)
{


}

QList<QObject *> TreeViewModel::data()
{
    return m_data;
}

void TreeViewModel::log()
{
    for(int i = 0; i < m_data.size(); ++i)
    {
        TreeElement *tmp = qobject_cast<TreeElement*>(m_data[i]);
        QString str = "[" + tmp->text() + ", " + QString::number(i) + "], level: " + QString::number(tmp->level()) +
                ", visible: " + QString::number((int)tmp->visible()) +
                ", open: " + QString::number((int)tmp->open()) + ", checked: " + QString::number((int)tmp->checked());
        qDebug() << str;
    }
}

void TreeViewModel::reset()
{
    for(int i = 0; i < m_data.size(); ++i)
    {
        TreeElement *tmp = qobject_cast<TreeElement*>(m_data[i]);
        tmp->reset();
    }
}

void TreeViewModel::initModel(DBWorker &dbWorker)
{
    /* Правила добавления элементов в дерево:
     * 1. Для верхних элементов в качестве предка указывается сама модель (this)
     * 2. Для остальных - указатель на другой элемент, в который они вложены
     * 3. Уровни вложенности, видимость, разворачиваемость вычисляются автоматически в классе TreeElement
     * 4. При добавлении созданных элементов в список (m_data) нужно добавлять их в том порядке,
     *    в каком они будут следовать друг за другом в полностью развернутом дереве
     *    (с учетом разворачиваемых элементов)
    */

    if(!this->m_data.isEmpty())
        this->clear();

    QObject *diagnosis = new TreeElement("Диагноз", this);
    QObject *nonMelanoma = new TreeElement("Не меланома", diagnosis);
    qobject_cast<TreeElement*>(nonMelanoma)->setTristate(true);
    QObjectList nonMelanomaDiagnoses;
    for(auto diag: dbWorker.nonMelanomaDiagnoses())
        nonMelanomaDiagnoses.push_back(new TreeElement(diag, nonMelanoma));


    QObject *melanoma = new TreeElement("Меланома", diagnosis);
    QObject *group = new TreeElement("Группа", this);
    QObjectList groupsList;
    for(auto g: dbWorker.groups())
        groupsList.push_back(new TreeElement(g, group));

    QObject *features = new TreeElement("Признаки", this);
    QObjectList featuresList;
    for(auto f: dbWorker.featureTypes())
        featuresList.push_back(new TreeElement(f, features));



     // **************** Добавление в список m_data *******************************************

     // метод push_back - для единичных элементов, append - для списков


    m_data.push_back(diagnosis);
        m_data.push_back(nonMelanoma);
            m_data.append(nonMelanomaDiagnoses);
        m_data.push_back(melanoma);


    m_data.push_back(group);
        m_data.append(groupsList);

    m_data.push_back(features);
        m_data.append(featuresList);


}

TreeElement *TreeViewModel::getTreeElement(const QString &text)
{
    QList<TreeElement*> treeElementList;
    for(auto d: this->m_data)
        treeElementList.push_back(qobject_cast<TreeElement*>(d));
    //auto el = std::find(treeElementList.begin(), treeElementList.end(), text);

    auto el = std::find(treeElementList.begin(), treeElementList.end(), text);
    if(el == treeElementList.end())
        return nullptr;
    return *el;
}

bool TreeViewModel::childrenAreChecked(const QString &parentElement)
{
    TreeElement *p = this->getTreeElement(parentElement);
//    for(auto ch: p->children())
//        if(qobject_cast<TreeElement*>(ch)->checked())
//            return true;
    return p->childrenAreChecked();
    return false;
}

bool TreeViewModel::allChildrenAreChecked(const QString &parentElement)
{
    TreeElement *p = this->getTreeElement(parentElement);
//    for(auto ch: p->children())
//        if(!qobject_cast<TreeElement*>(ch)->checked())
//            return false;
    return p->allChildrenAreChecked();
    return true;
}

QStringList TreeViewModel::getCheckedChildren(const QString &parentElement)
{
    TreeElement *p = this->getTreeElement(parentElement);
    QStringList checkedChildren;
    for(auto ch: p->children())
    {
        TreeElement *child = qobject_cast<TreeElement*>(ch);
        if(child->checked())
            checkedChildren.push_back(child->text());
    }
    return checkedChildren;
}

bool TreeViewModel::elementIsChecked(const QString &element)
{
    return this->getTreeElement(element)->checked();
}

int TreeViewModel::elementCheckedState(const QString &element)
{
    return this->getTreeElement(element)->checkState();
}

void TreeViewModel::clear()
{
    this->m_data.clear();
}



