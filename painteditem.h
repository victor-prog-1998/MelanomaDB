#ifndef PAINTEDITEM_H
#define PAINTEDITEM_H

#include <QQuickPaintedItem>


class PaintedItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(int lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)

    Q_PROPERTY(int x1 READ x1 WRITE setX1 NOTIFY x1Changed)
    Q_PROPERTY(int y1 READ y1 WRITE setY1 NOTIFY y1Changed)
    Q_PROPERTY(int x2 READ x2 WRITE setX2 NOTIFY x2Changed)
    Q_PROPERTY(int y2 READ y2 WRITE setY2 NOTIFY y2Changed)

public:
    PaintedItem(QQuickItem *parent = 0);

    virtual void paint(QPainter *painter);

    const QString &type();
    void setType(const QString& type);

    const QColor& color();
    void setColor(const QColor& color);

    int lineWidth();
    void setLineWidth(int lineWidth);

    int x1();
    void setX1(int x1);

    int y1();
    void setY1(int y1);

    int x2();
    void setX2(int x2);

    int y2();
    void setY2(int y2);

private:
    QString m_type = "Линия";

    QColor m_color;

    int m_lineWidth;

    int m_x1;
    int m_y1;

    int m_x2;
    int m_y2;
signals:



    void typeChanged();
    void colorChanged();
    void lineWidthChanged();

    void x1Changed();
    void y1Changed();
    void x2Changed();
    void y2Changed();

};

#endif // PAINTEDITEM_H
