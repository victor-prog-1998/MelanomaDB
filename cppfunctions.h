#ifndef CPPFUNCTIONS_H
#define CPPFUNCTIONS_H

#include <QObject>

class CppFunctions : public QObject
{
    Q_OBJECT
public:
    explicit CppFunctions(QObject *parent = nullptr);

    // по виду ссылки на изображение опеределяет, является ли оно загруженным из провайдера (ImageProvider)
    static bool imageIsFromProvider(const QString& source);



signals:

public slots:
};

#endif // CPPFUNCTIONS_H
