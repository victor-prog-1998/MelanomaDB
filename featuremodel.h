#ifndef FEATUREMODEL_H
#define FEATUREMODEL_H

#include <QObject>

class FeatureModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString featureType READ featureType NOTIFY propertyChanged)
    Q_PROPERTY(QString color READ color NOTIFY propertyChanged)
    Q_PROPERTY(QList<QObject*> markers READ markers NOTIFY propertyChanged)

public:
    explicit FeatureModel(const QString& featureType, const QString& color,
                          const QList<QObject*>& markers, QObject *parent = nullptr);
    const QString& featureType() const;
    const QString& color() const;
    const QList<QObject*>& markers() const;
private:
    QString m_featureType;
    QString m_color;
    QList<QObject*> m_markers;


signals:
    void propertyChanged();
public slots:
};

#endif // FEATUREMODEL_H
