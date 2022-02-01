#include "featuremodel.h"
#include <QDebug>

FeatureModel::FeatureModel(const QString &featureType, const QString &color, const QList<QObject *> &markers, QObject *parent) :
    QObject(parent), m_featureType(featureType), m_color(color), m_markers(markers)
{

}

const QString &FeatureModel::featureType() const
{
    return this->m_featureType;
}

const QString &FeatureModel::color() const
{
    return this->m_color;
}

const QList<QObject *> &FeatureModel::markers() const
{
    return this->m_markers;
}
