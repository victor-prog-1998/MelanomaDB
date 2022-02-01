#include "cppfunctions.h"



CppFunctions::CppFunctions(QObject *parent) : QObject(parent)
{

}

bool CppFunctions::imageIsFromProvider(const QString &source)
{
    // Пример ссылки на изображение из провайдера  "image://provider/image_1"
    // Результат - лог. выражение: является ли подстрока из первых 8 символов строкой "image://"
    return (source.left(8) == "image://");
}

