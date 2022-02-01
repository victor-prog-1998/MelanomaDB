#include "imageprovider.h"
#include <QDebug>

ImageProvider::ImageProvider() : QQuickImageProvider(QQuickImageProvider::Image)
{

}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(size)
    Q_UNUSED(requestedSize)
    return m_imagesHash[id];
}

void ImageProvider::add(const QString &id, const QImage &image, const QByteArray &byteArray)
{
    this->m_imagesHash[id] = image;
    this->m_byteArraysHash[id] = byteArray;
}

void ImageProvider::clear()
{
    this->m_imagesHash.clear();
    this->m_byteArraysHash.clear();
    qDebug() << "imageprovider::clear()";
}

const QByteArray &ImageProvider::getByteArray(const QString &source)
{
    // Пример ссылки на изображение из провайдера  "image://provider/image_1"
    QString id = source.split('/').last();
    return this->m_byteArraysHash[id];
}
