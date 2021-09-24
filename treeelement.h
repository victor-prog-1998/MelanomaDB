#ifndef TREEELEMENT_H
#define TREEELEMENT_H

#include <QObject>

class TreeElement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text NOTIFY textChanged)
    Q_PROPERTY(bool checked READ checked WRITE setChecked NOTIFY checkedChanged)
    Q_PROPERTY(int level READ level NOTIFY levelChanged)
    Q_PROPERTY(bool open READ open WRITE setOpen NOTIFY openChanged)
    Q_PROPERTY(bool expandable READ expandable NOTIFY expandableChanged)
    Q_PROPERTY(bool visible READ visible NOTIFY visibleChanged)
    Q_PROPERTY(bool tristate READ tristate NOTIFY tristateChanged)
    Q_PROPERTY(int checkState READ checkState WRITE setCheckState NOTIFY checkStateChanged)





public:
    explicit TreeElement(const QString& text, QObject *parent = nullptr);

    const QString& text() const;
    bool checked() const;
    int level() const;
    bool open() const;
    bool expandable() const;
    bool visible() const;
    bool tristate() const;
    int checkState() const;


    void setChecked(bool checked);
    void setOpen(bool open);

    void setVisible(bool visible);
    void setTristate(bool tristate);
    void setCheckState(int checkState);


    void reset();

    bool isChecked();
    bool childrenAreChecked();
    bool allChildrenAreChecked();
    QList<TreeElement*> getCheckedChildren();

    // Оператор для возможности поиска при помощи std::find элемента списка с указанным текстом
    friend bool operator ==(TreeElement* el1, const QString& text);

private:

    QString m_text;
    // bool m_checked = false;
    int m_level = 0;
    bool m_expandable = false;
    bool m_open = false;
    bool m_visible;
    bool m_tristate = false;  // задается true для разворачиваемых элементов,
                              // у которых есть CheckBox для установки состояния дочерних элементов
    int m_checkState = Qt::Unchecked;

signals:
    void textChanged();
    void checkedChanged();
    void levelChanged();
    void openChanged();
    void expandableChanged();
    void visibleChanged();
    void tristateChanged();
    void checkStateChanged();

public slots:
};

#endif // TREEELEMENT_H
