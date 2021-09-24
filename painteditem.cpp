#include "painteditem.h"
#include <QPainter>

PaintedItem::PaintedItem(QQuickItem *parent): QQuickPaintedItem(parent)
{

}

void PaintedItem::paint(QPainter *painter)
{
    painter->setPen(QPen(m_color, m_lineWidth));

    if(m_type == "Линия")
    {
        painter->drawLine(m_x1, m_y1, m_x2, m_y2);
    }

    else if(m_type == "Окружность")
    {
        double radius = hypot(m_x2 - m_x1, m_y2 - m_y1);
        painter->drawEllipse(QPointF(m_x1, m_y1), radius, radius);
    }

    else if(m_type == "Прямоугольник")
    {
        int x1 = qMin(m_x1, m_x2);
        int x2 = qMax(m_x1, m_x2);
        int y1 = qMin(m_y1, m_y2);
        int y2 = qMax(m_y1, m_y2);
        painter->drawRect(x1, y1, x2 - x1, y2 - y1);
    }

    else if(m_type == "Стрелка")
    {
        double ratio = 8; // Отношение длины стрелки к половине ширины ее хвоста

        QPoint p1(m_x1 - round((m_y1 - m_y2) / ratio), m_y1 - round((m_x2 - m_x1) / ratio));
        QPoint p2(p1.x() + round((m_x2 - m_x1) * (ratio - 2) / ratio),
                  p1.y() + round((m_y2 - m_y1) * (ratio - 2) / ratio));

        QPoint p3(p2.x() + p1.x() - m_x1, p2.y() + p1.y() - m_y1);
        QPoint p4(m_x2, m_y2);
        QPoint p5(p3.x() + (p2.x() - p3.x()) * 4, p3.y() + (p2.y() - p3.y()) * 4);

        QPoint p6(p5.x() + p3.x() - p2.x(), p5.y() + p3.y() - p2.y());
        QPoint p7(2 * m_x1 - p1.x(), 2 * m_y1 - p1.y());

        QPolygon poly;

        poly << p1 << p2 << p3 << p4 << p5 << p6 << p7;

        painter->drawPolygon(poly);

    }


}

const QString& PaintedItem::type()
{
    return m_type;
}

void PaintedItem::setType(const QString& type)
{
    m_type = type;
    this->update();
    emit typeChanged();
}

const QColor &PaintedItem::color()
{
    return this->m_color;
}

void PaintedItem::setColor(const QColor &color)
{
    this->m_color = color;
    this->update();
    emit colorChanged();
}

int PaintedItem::lineWidth()
{
    return m_lineWidth;
}

void PaintedItem::setLineWidth(int lineWidth)
{
    m_lineWidth = lineWidth;
    this->update();
    emit lineWidthChanged();
}

int PaintedItem::x1()
{
    return m_x1;
}

void PaintedItem::setX1(int x1)
{
    m_x1 = x1;
    this->update();
    emit x1Changed();
}

int PaintedItem::y1()
{
    return m_y1;
}

void PaintedItem::setY1(int y1)
{
    m_y1 = y1;
    this->update();
    emit y1Changed();
}

int PaintedItem::x2()
{
    return m_x2;
}

void PaintedItem::setX2(int x2)
{
    m_x2 = x2;
    this->update();
    emit x2Changed();
}

int PaintedItem::y2()
{
    return m_y2;
}

void PaintedItem::setY2(int y2)
{
    m_y2 = y2;
    this->update();
    emit y2Changed();
}
