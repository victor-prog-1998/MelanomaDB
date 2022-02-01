#include "treeelement.h"
#include <QDebug>

TreeElement::TreeElement(const QString& text, QObject *parent) : QObject(parent),
    m_text(text)
{

    if(parent)
    {
        TreeElement *elem = qobject_cast<TreeElement*>(parent); // попытка приведения предка к TreeElement
        if(elem)  // Если удалось привести к TreeElement* т.е. предок - другой элемент дерева
        {
            // Скрываем элемент, т.к. изначально должны быть видны только верхние элементы дерева
            this->m_visible = false;
            // Делаем родительский элемент разворачиваемым
            elem->m_expandable = true;
            // Повышаем уровень вложенности текущего элемента на 1 по отношению к предку
            this->m_level = elem->level() + 1;
        }
        else // Если предок - сама модель (т.е. текущий элемент на верхнем уровне)
        {
            this->m_visible = true;
            // Устанавливать разворачиваемость в true здесь не нужно.
            // Это производится в конструкторах дочерних классов (блок if(elem))
        }
    }
}

void TreeElement::setChecked(bool checked)
{
    // m_checked = checked ? Qt::Checked : Qt::Unchecked;
    this->m_checkState = checked ? Qt::Checked : Qt::Unchecked;
    emit checkedChanged();
    emit checkStateChanged();
}

void TreeElement::setOpen(bool open)
{
    m_open = open;
    emit openChanged();
    for(auto ch: this->children())
    {
        TreeElement *child = qobject_cast<TreeElement*>(ch);
        child->setVisible(open);
    }
}

void TreeElement::setVisible(bool visible)
{
    this->m_visible = visible;
    emit this->visibleChanged();
    if(this->m_expandable && this->m_open)
    {
        for(auto ch: this->children())
        {
            TreeElement *child = qobject_cast<TreeElement*>(ch);
            child->setVisible(visible);
        }
    }
}

void TreeElement::reset()
{
    if(this->expandable())
    {
        this->m_open = false;
        emit this->openChanged();
    }
    TreeElement *elem = qobject_cast<TreeElement*>(this->parent());
    if(elem) // если предок - другой элемент дерева
    {
        this->m_visible = false;
        emit this->visibleChanged();
    }
    this->setChecked(false);
}

void TreeElement::setTristate(bool tristate)
{
    this->m_tristate = tristate;
    emit this->tristateChanged();
}

void TreeElement::setCheckState(int checkState)
{
    this->m_checkState = checkState;
    emit checkStateChanged();
    emit checkedChanged();
    if(this->tristate()) // значит нужно установить в checked или unchecked
                         // дочерние элементы
    {
        for(auto ch: this->children())
            qobject_cast<TreeElement*>(ch)->setCheckState(checkState);
    }

    TreeElement *p = qobject_cast<TreeElement*>(this->parent());
    if(p->tristate())
    {
        int state = p->allChildrenAreChecked() ? Qt::Checked : p->childrenAreChecked() ? Qt::PartiallyChecked : Qt::Unchecked;
        // здесь не вызываем метод setChecked для предка, чтобы не зациклиться
        // (см. блок if для установки состояния дочерних элементов)
        p->m_checkState = state;
        p->checkStateChanged();
        p->checkedChanged();
    }

}

bool TreeElement::isChecked()
{
    return this->m_checkState == Qt::Checked;
}

bool TreeElement::childrenAreChecked()
{
    for(auto ch: this->children())
    {
        if(qobject_cast<TreeElement*>(ch)->checked())
            return true;
    }
    return false;
}

bool TreeElement::allChildrenAreChecked()
{
    for(auto ch: this->children())
    {
        if(!qobject_cast<TreeElement*>(ch)->checked())
            return false;
    }
    return true;
}

QList<TreeElement *> TreeElement::getCheckedChildren()
{
    QList<TreeElement*> checkedChildren;
    for(auto ch: this->children())
    {
        TreeElement* child =  qobject_cast<TreeElement*>(ch);
        if(child->checked())
            checkedChildren.push_back(child);
    }
    return checkedChildren;
}

bool operator ==(TreeElement *el1, const QString& text)
{
    return el1->m_text == text;
}


const QString& TreeElement::text() const
{
    return m_text;
}

bool TreeElement::checked() const
{
    return m_checkState == Qt::Checked;
}

int TreeElement::level() const
{
    return m_level;
}

bool TreeElement::open() const
{
    return m_open;
}

bool TreeElement::expandable() const
{
    return m_expandable;
}

bool TreeElement::visible() const
{
    return m_visible;
}

bool TreeElement::tristate() const
{
    return this->m_tristate;
}

int TreeElement::checkState() const
{
    return this->m_checkState;
}
