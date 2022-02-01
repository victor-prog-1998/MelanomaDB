#include "markermodel.h"

MarkerModel::MarkerModel(int featureId, const QString &type, int lineWidth, int x1, int y1, int x2, int y2, QObject *parent) :
    QObject(parent), m_featureId(featureId), m_type(type), m_lineWidth(lineWidth), m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2)
{

}

int MarkerModel::featureId() const
{
    return this->m_featureId;
}

const QString& MarkerModel::type() const
{
    return this->m_type;
}


int MarkerModel::lineWidth() const
{
    return this->m_lineWidth;
}


int MarkerModel::x1() const
{
    return this->m_x1;
}


int MarkerModel::y1() const
{
    return this->m_y1;
}


int MarkerModel::x2() const
{
    return this->m_x2;
}


int MarkerModel::y2() const
{
    return this->m_y2;
}
