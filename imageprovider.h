#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QHash>


class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    void add(const QString& id, const QImage& image, const QByteArray& byteArray);
    void clear();
    const QByteArray& getByteArray(const QString& source);

private:
    QHash<QString, QImage> m_imagesHash;   // хеш таблица из пар "id - изображение" (id - это его название в url)
    QHash<QString, QByteArray> m_byteArraysHash;
};

#endif // IMAGEPROVIDER_H
