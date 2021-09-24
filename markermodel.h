#ifndef MARKERMODEL_H
#define MARKERMODEL_H

#include <QObject>

class MarkerModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int featureId READ featureId NOTIFY propertyChanged)
    Q_PROPERTY(QString type READ type NOTIFY propertyChanged)
    Q_PROPERTY(int lineWidth READ lineWidth NOTIFY propertyChanged)
    Q_PROPERTY(int x1 READ x1 NOTIFY propertyChanged)
    Q_PROPERTY(int y1 READ y1 NOTIFY propertyChanged)
    Q_PROPERTY(int x2 READ x2 NOTIFY propertyChanged)
    Q_PROPERTY(int y2 READ y2 NOTIFY propertyChanged)

public:
    explicit MarkerModel(int featureId, const QString& type, int lineWidth, int x1, int y1, int x2, int y2,
                         QObject *parent = nullptr);

    void f();

    int featureId() const;
    const QString& type() const;
    int lineWidth() const;
    int x1() const;
    int y1() const;
    int x2() const;
    int y2() const;

//    void setFeatureId(int featureId);
//    void setType(const QString& type);
//    void setLineWidth(int lineWidth);
//    void setX1(int x1);
//    void setY1(int y1);
//    void setX2(int x2);
//    void setY2(int y2);


private:

    int m_featureId;
    QString m_type;
    int m_lineWidth;
    int m_x1;
    int m_y1;
    int m_x2;
    int m_y2;


signals:

    void propertyChanged();

public slots:
};

#endif // MARKERMODEL_H
