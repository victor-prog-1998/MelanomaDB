#include "imagemodel.h"

ImageModel::ImageModel(QObject *parent): QObject(parent)
{

}

//ImageModel::ImageModel(int id, const QString &imageSource, const QString& addDatetime, const QString& lastEditDatetime,
//                                   const QString& group,
//                                   const QString& diagnosis,
//                                   QObject *parent) : QObject(parent),
//    m_id(id), m_imageSource(imageSource), m_addDatetime(addDatetime),
//    m_lastEditDatetime(lastEditDatetime), m_group(group),
//    m_diagnosis(diagnosis)
//{

//}

int ImageModel::id() const
{
    return this->m_id;
}

const QString &ImageModel::imageSource() const
{
    return this->m_imageSource;
}

//const QString &ImageModel::addDatetime() const
//{
//    return this->m_addDatetime;
//}

//const QString &ImageModel::lastEditDatetime() const
//{
//    return this->m_lastEditDatetime;
//}

const QString &ImageModel::diagnosis() const
{
    return this->m_diagnosis;
}

const QList<QObject *> &ImageModel::features() const
{
    return this->m_features;
}

const QList<QObject *> &ImageModel::markers() const
{
    return this->m_markers;
}

bool ImageModel::error() const
{
    return this->m_error;
}

void ImageModel::setData(int id, const QString &imageSource, const QString &group, const QString &diagnosis)
{
    this->m_id = id;
    this->m_imageSource = imageSource;
    this->m_group = group;
    this->m_diagnosis = diagnosis;
}

void ImageModel::setFeatures(const QList<QObject *> &features)
{
    this->m_features = features;
}

void ImageModel::setMarkers(const QList<QObject *> &markers)
{
    this->m_markers = markers;
}

void ImageModel::setError(bool error)
{
    this->m_error = error;
}

const QString &ImageModel::group() const
{
    return this->m_group;
}
