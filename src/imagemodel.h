#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QObject>

class ImageModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id NOTIFY propertyChanged)
    Q_PROPERTY(QString imageSource READ imageSource NOTIFY propertyChanged)
    //Q_PROPERTY(QString addDatetime READ addDatetime NOTIFY propertyChanged)
    //Q_PROPERTY(QString lastEditDatetime READ lastEditDatetime NOTIFY propertyChanged)
    Q_PROPERTY(QString group READ group NOTIFY propertyChanged)
    Q_PROPERTY(QString diagnosis READ diagnosis NOTIFY propertyChanged)
    Q_PROPERTY(QList<QObject*> features READ features NOTIFY propertyChanged)
    Q_PROPERTY(QList<QObject*> markers READ markers NOTIFY propertyChanged)
    Q_PROPERTY(bool error READ error NOTIFY propertyChanged)

public:
    explicit ImageModel(QObject *parent = nullptr);
//    explicit ImageModel(int id, const QString& imageSource, const QString& addDatetime, const QString& lastEditDatetime, const QString& group,
//                              const QString& diagnosis,
//                              QObject *parent = nullptr);

    int id() const;
    const QString& imageSource() const;
    //const QString& addDatetime() const;
    //const QString& lastEditDatetime() const;
    const QString& group() const;
    const QString& diagnosis() const;
    const QList<QObject*>& features() const;
    const QList<QObject*>& markers() const;
    bool error() const;




    void setData(int id, const QString& imageSource, const QString& group, const QString& diagnosis);
    void setFeatures(const QList<QObject*>& features);
    void setMarkers(const QList<QObject*>& markers);
    void setError(bool error);



signals:

    void propertyChanged();

private:
    int m_id;
    QString m_imageSource;
    //QString m_addDatetime;
    //QString m_lastEditDatetime;
    QString m_group;
    QString m_diagnosis;
    QList<QObject*> m_features;
    QList<QObject*> m_markers;
    bool m_error = false;

public slots:
};

#endif // IMAGEMODEL_H
