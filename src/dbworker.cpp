#include "dbworker.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QByteArray>
#include <QBuffer>
#include <QImage>
#include <QDateTime>
#include <functional>

#include "cppfunctions.h"
#include "tablerowmodel.h"
#include "imagemodel.h"
#include "featuremodel.h"
#include "markermodel.h"
#include "queryresultmodel.h"

DBWorker::DBWorker(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("melanoma_db");

    if(!m_db.open())
    {
        qDebug() << "Ошибка подключения к базе данных";
    }
    else
    {
        if(m_db.tables().empty())
            this->createTables();
        this->fillReferenceTables();
    }
}

void DBWorker::createTables()
{
    QStringList columnsList;

    // Diagnosis
    columnsList.push_back(this->sqlIdColumn("TINYINT"));
    columnsList.push_back("diagnosis VARCHAR(100)");
    if(!createTable("Diagnosis", columnsList))
        qDebug() << "Ошибка при создании таблицы \"Диагноз\"";
    else
        qDebug() << "Таблица \"Диагноз\" создана";
    columnsList.clear();

    // Group
    columnsList.push_back(this->sqlIdColumn("SMALLINT"));
    columnsList.push_back("name VARCHAR(100)");
    if(!createTable("\"Group\"", columnsList))
        qDebug() << "Ошибка при создании таблицы \"Группа\"";
    else
        qDebug() << "Таблица \"Группа\" создана";

    columnsList.clear();

    // Image
    columnsList.push_back(this->sqlIdColumn());
    columnsList.push_back("add_datetime DATETIME");
    columnsList.push_back("last_edit_datetime DATETIME");
    columnsList.push_back("group_id SMALLINT UNSIGNED");
    columnsList.push_back("diagnosis_id TINYINT UNSIGNED");
    columnsList.push_back("image BLOB");

    if(!createTable("Image", columnsList))
        qDebug() << "Ошибка при создании таблицы \"Изображение\"";
    else
        qDebug() << "Таблица \"Изображение\" создана";

    columnsList.clear();

    // FeatureType
    columnsList.push_back(this->sqlIdColumn("TINYINT"));
    columnsList.push_back("type VARCHAR(50)");
    if(!createTable("FeatureType", columnsList))
        qDebug() << "Ошибка при создании таблицы \"Тип признака\"";
    else
        qDebug() << "Таблица \"Тип признака\" создана";

    columnsList.clear();

    // Feature
    columnsList.push_back(this->sqlIdColumn());
    columnsList.push_back("image_id INTEGER UNSIGNED");
    columnsList.push_back("feature_type_id TINYINT UNSIGNED");
    columnsList.push_back("color VARCHAR(7)");
    if(!createTable("Feature", columnsList))
        qDebug() << "Ошибка при создании таблицы \"Признак\"";
    else
        qDebug() << "Таблица \"Признак\" создана";

    columnsList.clear();

    // MarkerType
    columnsList.push_back(this->sqlIdColumn("TINYINT"));
    columnsList.push_back("type VARCHAR(40)");
    if(!createTable("MarkerType", columnsList))
        qDebug() << "Ошибка при создании таблицы \"Тип маркера\"";
    else
        qDebug() << "Таблица \"Тип маркера\" создана";

    columnsList.clear();

    // Marker
    columnsList.push_back(this->sqlIdColumn());
    columnsList.push_back("feature_id INTEGER UNSIGNED");
    columnsList.push_back("marker_type_id TINYINT UNSIGNED");
    columnsList.push_back("line_width TINYINT UNSIGNED");
    columnsList.push_back("x1 SMALLINT UNSIGNED");
    columnsList.push_back("y1 SMALLINT UNSIGNED");
    columnsList.push_back("x2 SMALLINT UNSIGNED");
    columnsList.push_back("y2 SMALLINT UNSIGNED");

    if(!createTable("Marker", columnsList))
        qDebug() << "Ошибка при создании таблицы \"Маркер\"";
    else
        qDebug() << "Таблица \"Маркер\" создана";


}

bool DBWorker::createTable(const QString& title, const QString& columns)
{
    QString query = "CREATE TABLE IF NOT EXISTS " + title + " (" + columns + ')';
    QSqlQuery q(this->m_db);
    q.prepare(query);
    return q.exec();
}

bool DBWorker::createTable(const QString& title, const QStringList& columns)
{
    QString columnsString;
    for(int i = 0; i < columns.size(); ++i)
    {
        columnsString += columns[i];
        if(i < (columns.size() - 1))
            columnsString += ", ";
    }
    return this->createTable(title, columnsString);
}

bool DBWorker::tableIsEmpty(const QString &tableName)
{
    QSqlQuery q(this->m_db);
    q.prepare("SELECT COUNT(*) FROM " + tableName);
    if(!q.exec())
    {
        qDebug() << "Ошибка при проверке пустоты таблицы " + tableName;
        return false;
    }
    q.next();
    return (q.value(0).toInt() == 0);
}

bool DBWorker::deleteRecords(const QString &tableName, const QString &conditionField, const QVariant &value)
{
    QSqlQuery q(this->m_db);
    q.prepare("DELETE FROM " + tableName + " WHERE " + conditionField + " = ?");
    q.addBindValue(value);
    if(q.exec())
    {
        qDebug() << "Записи из таблицы " << tableName << ", где " << conditionField << " = " << value << " удалены";
        return true;
    }
    qDebug() << "Не удалось удалить записи из таблицы " << tableName << ", где " << conditionField << " = " << value;
    return false;
}

QList<QObject *> DBWorker::imagesModel(const QString &conditions)
{
    QString query = "SELECT Image.id, Image.add_datetime, Image.last_edit_datetime, \"Group\".name, "
                    "Diagnosis.diagnosis FROM Image INNER JOIN "
                    "\"Group\" ON Image.group_id = \"Group\".id INNER JOIN "
                    "Diagnosis ON Image.diagnosis_id = Diagnosis.id ";
    query += conditions;


    QList<QObject*> model;

    QSqlQuery q(this->m_db);
    q.prepare(query);

    q.exec();

    while(q.next())
    {
        int id = q.value(0).toInt();
        QDateTime addDatetime = q.value(1).toDateTime();
        QDateTime lastEditDatetime = q.value(2).toDateTime();
        QString group = q.value(3).toString();
        QString diagnosis = q.value(4).toString();



        QStringList row;
        row.push_back(QString::number(id));
        row.push_back(addDatetime.toString("dd.MM.yyyy hh:mm"));
        row.push_back(lastEditDatetime.toString("dd.MM.yyyy hh:mm"));
        row.push_back(group);
        row.push_back(diagnosis);
        QString features;
        for(auto f: this->distinctFeatures(id))
            features += f[0] + " ";
        row.push_back(features);

        row.push_back("<buttons>");




        model.push_back(new TableRowModel(row, this));
    }

    return model;
}


void DBWorker::fillReferenceTables()
{
    if(this->tableIsEmpty("Diagnosis"))
        this->fillDiagnosisTable();
    if(this->tableIsEmpty("\"Group\""))
        this->fillGroupTable();
    if(this->tableIsEmpty("FeatureType"))
        this->fillFeatureTypeTable();
    if(this->tableIsEmpty("MarkerType"))
        this->fillMarkerTypeTable();
}

void DBWorker::fillDiagnosisTable()
{
    QStringList list =
    {
        "Меланома",
        "Н",
        "Н/дисп",
        "Д/ф",
        "К",
        "А",
        "БКР",
        "М?",
    };

    QSqlQuery q(this->m_db);
    int id = 1;
    for(auto str: list)
    {
        q.prepare("INSERT INTO Diagnosis (id, diagnosis) VALUES(?,?)");
        q.addBindValue(id);
        q.addBindValue(str);
        q.exec();
        ++id;
    }
}

void DBWorker::fillGroupTable()
{
    QStringList list =
    {
        "402",
        "401",
        "399",
        "398"
    };

    QSqlQuery q(this->m_db);
    int id = 1;
    for(auto str: list)
    {
        q.prepare("INSERT INTO \"Group\" (id, name) VALUES(?,?)");
        q.addBindValue(id);
        q.addBindValue(str);
        q.exec();
        ++id;
    }
}

void DBWorker::fillFeatureTypeTable()
{
    QStringList list =
    {
        "Точка",
        "Линия",
        "Область"
    };
    QSqlQuery q(this->m_db);
    int id = 1;
    for(auto str: list)
    {
        q.prepare("INSERT INTO FeatureType (id, type) VALUES (?,?)");
        q.addBindValue(id);
        q.addBindValue(str);
        q.exec();
        ++id;
    }
}

void DBWorker::fillMarkerTypeTable()
{
    QStringList list =
    {
        "Линия",
        "Стрелка",
        "Окружность",
        "Прямоугольник"
    };
    QSqlQuery q(this->m_db);
    int id = 1;
    for(auto str: list)
    {
        q.prepare("INSERT INTO MarkerType (id, type) VALUES (?,?)");
        q.addBindValue(id);
        q.addBindValue(str);
        q.exec();
        ++id;
    }
}



QStringList DBWorker::nonMelanomaDiagnoses()
{
    QStringList list;
    QSqlQuery q(this->m_db);
    q.prepare("SELECT diagnosis FROM Diagnosis ORDER BY diagnosis");
    q.exec();
    while(q.next())
    {
        QString str = q.value(0).toString();
        if(str != "Меланома")
            list.push_back(str);
    }
    return list;
}

QStringList DBWorker::groups()
{
    QStringList list;
    QSqlQuery q(this->m_db);
    q.prepare("SELECT name FROM \"Group\" ORDER BY name");
    q.exec();
    while(q.next())
        list.push_back(q.value(0).toString());
    return list;
}

QStringList DBWorker::featureTypes()
{
    QStringList list;
    QSqlQuery q(this->m_db);
    q.prepare("SELECT type FROM FeatureType");
    q.exec();
    while(q.next())
        list.push_back(q.value(0).toString());
    return list;
}

QStringList DBWorker::markerTypes()
{
    QStringList list;
    QSqlQuery q(this->m_db);
    q.prepare("SELECT type FROM MarkerType");
    q.exec();
    while(q.next())
        list.push_back(q.value(0).toString());
    return list;
}

int DBWorker::insertNewDiagnosis(const QString& diagnosis)
{
    int id = this->getIDForInsert("Diagnosis");
    QSqlQuery q(this->m_db);
    q.prepare("INSERT INTO Diagnosis (id, diagnosis) VALUES (?,?)");
    q.addBindValue(id);
    q.addBindValue(diagnosis);
    if(!q.exec())
    {
        qDebug() << "Ошибка при добавлении нового диагноза \"" << diagnosis << '\"';
        return 0;
    }
    return id;
}

int DBWorker::insertNewGroup(const QString& group)
{
    int id = this->getIDForInsert("\"Group\"");
    QSqlQuery q(this->m_db);
    q.prepare("INSERT INTO \"Group\" (id, name) VALUES (?,?)");
    q.addBindValue(id);
    q.addBindValue(group);
    if(!q.exec())
    {
        qDebug() << "Ошибка при добавлении новой группы \"" << group << '\"';
        return 0;
    }
    return id;
}

int DBWorker::insertImage(const QString &group, const QString &diagnosis, const QString &imageUrl, int originalId )
{

    int id = this->getIDForInsert("Image");
    QDateTime currentDateTime = QDateTime::currentDateTime();

    std::function<void()> errorDebug = []() {qDebug() << "Ошибка при добавлении изображения";};

    bool groupIsNew = false;
    int groupId = this->getIDByFieldData("\"Group\"", "name", group);
    if(groupId == 0)
    {
        groupId = this->insertNewGroup(group);
        groupIsNew = true;
    }
    if(groupId == 0)
    {
        errorDebug();
        return 0;
    }

    bool diagnosisIsNew = false;
    int diagnosisId = this->getIDByFieldData("Diagnosis", "diagnosis", diagnosis);
    if(diagnosisId == 0)
    {
        diagnosisId = this->insertNewDiagnosis(diagnosis);
        diagnosisIsNew = true;
    }
    if(diagnosisId == 0) // такое возможно если диагноз оказался новым и его не удалось записать в таблицу Diagnosis
    {
        errorDebug();
        // +Желательно удалить новую группу, если она добавлялась
        if(groupIsNew)
            this->deleteRecords("\"Group\"", "id", groupId);
        return 0;
    }

    QByteArray byteArray = this->imageToByteArray(imageUrl);

    if(byteArray.isEmpty()) //
    {
        errorDebug();
        // + удаление новых группы и диагноза
        if(groupIsNew)
            this->deleteRecords("\"Group\"", "id", groupId);
        if(diagnosisIsNew)
            this->deleteRecords("Diagnosis", "id", diagnosisId);
        return 0;
    }


    QSqlQuery q(this->m_db);

    // Image
//    columnsList.push_back(this->sqlIdColumn());
//    columnsList.push_back("add_datetime DATETIME");
//    columnsList.push_back("group_id SMALLINT UNSIGNED");
//    columnsList.push_back("diagnosis_id TINYINT UNSIGNED");
//    columnsList.push_back("image BLOB");

    q.prepare("INSERT INTO IMAGE (id, add_datetime, last_edit_datetime, "
              "group_id, diagnosis_id, image) VALUES (?,?,?,?,?,?)");
    q.addBindValue(id);
    if(originalId == 0)  // если изображение просто добавляется в базу
    {
        q.addBindValue(currentDateTime);
        qDebug() << "Просто добавление";
    }
    else // случай редактирования, когда создается "новая" запись
    {
        qDebug() << "Создание временной записи";
        QSqlQuery q1(this->m_db);
        q1.prepare("SELECT add_datetime FROM Image WHERE id = ?");
        q1.addBindValue(originalId);
        q1.exec();
        q1.next();
        QDateTime addDatetime = q1.value(0).toDateTime();
        q.addBindValue(addDatetime);
    }

    q.addBindValue(currentDateTime); // при добавлении это значение совпадает со временем добавления
    q.addBindValue(groupId);
    q.addBindValue(diagnosisId);
    q.addBindValue(byteArray);

    if(!q.exec())
    {
        errorDebug();
        // + удаление новых группы и диагноза
        if(groupIsNew)
            this->deleteRecords("\"Group\"", "id", groupId);
        if(diagnosisIsNew)
            this->deleteRecords("Diagnosis", "id", diagnosisId);
        return 0;
    }

    return id;
}

int DBWorker::insertFeature(int imageId, const QString& type, const QString& color)
{
    /*
     * // FeatureType
    columnsList.push_back(this->sqlIdColumn("TINYINT"));
    columnsList.push_back("type VARCHAR(50)");
    if(!createTable("FeatureType", columnsList))
        qDebug() << "Ошибка при создании таблицы \"Тип признака\"";
    else
        qDebug() << "Таблица \"Тип признака\" создана";

    columnsList.clear();

    // Feature
    columnsList.push_back(this->sqlIdColumn());
    columnsList.push_back("image_id INTEGER UNSIGNED");
    columnsList.push_back("feature_type_id TINYINT UNSIGNED");
    if(!createTable("Feature", columnsList))
        qDebug() << "Ошибка при создании таблицы \"Признак\"";
    else
        qDebug() << "Таблица \"Признак\" создана"; */

    int id = this->getIDForInsert("Feature");
    int typeId = this->getIDByFieldData("FeatureType", "type", type);


    QSqlQuery q(this->m_db);
    q.prepare("INSERT INTO Feature (id, image_id, feature_type_id, color) VALUES (?,?,?,?)");
    q.addBindValue(id);
    q.addBindValue(imageId);
    q.addBindValue(typeId);
    color == "" ? q.addBindValue(QVariant(QVariant::String)) : q.addBindValue(color);
    if(!q.exec())
    {
        qDebug() << "Ошибка при добавлении признака";
        return 0;
    }
    qDebug() << "Признак добавлен";


    return id;
}

void DBWorker::insertMarker(int featureId, const QString& type, int lineWidth, int x1, int y1, int x2, int y2)
{
//    columnsList.push_back(this->sqlIdColumn());
//    columnsList.push_back("feature_id INTEGER UNSIGNED");
//    columnsList.push_back("marker_type_id TINYINT UNSIGNED");
//    columnsList.push_back("line_width TINYINT UNSIGNED");
//    columnsList.push_back("x1 SMALLINT UNSIGNED");
//    columnsList.push_back("y1 SMALLINT UNSIGNED");
//    columnsList.push_back("x2 SMALLINT UNSIGNED");
//    columnsList.push_back("y2 SMALLINT UNSIGNED");

    int id = this->getIDForInsert("Marker");
    int typeId = this->getIDByFieldData("MarkerType", "type", type);

    QSqlQuery q(this->m_db);
    q.prepare("INSERT INTO Marker (id, feature_id, marker_type_id, line_width, x1, y1, x2, y2) VALUES (?,?,?,?,?,?,?,?)");
    q.addBindValue(id);
    q.addBindValue(featureId);
    q.addBindValue(typeId);
    q.addBindValue(lineWidth);
    q.addBindValue(x1);
    q.addBindValue(y1);
    q.addBindValue(x2);
    q.addBindValue(y2);

    if(!q.exec())
        qDebug() << "Не удалось добавить маркер: id = " << id << ", featureId = " << featureId;
}



void DBWorker::deleteImage(int id)
{
    QSqlQuery q(this->m_db);
    q.prepare("DELETE FROM Image WHERE id = ?");
    q.addBindValue(id);
    if(!q.exec())
    {
        qDebug() << "Ошибка удаления изображения " << id;
        return;
    }
        qDebug() << "Изображение " << id << " удалено";


    // Сначала удаляем маркеры и только затем признаки
    // Маркеры
    q.prepare("DELETE FROM Marker WHERE feature_id IN (SELECT id FROM Feature WHERE image_id = ?)");
    q.addBindValue(id);
    if(!q.exec())
    {
        qDebug() << "Не удалось удалить маркеры";
        return;
    }
    qDebug() << "Маркеры изображения " << id << " удалены";

    // Признаки
    q.prepare("DELETE FROM Feature WHERE image_id = ?");
    q.addBindValue(id);
    if(!q.exec())
    {
        qDebug() << "Ошибка удаления признаков изображения " << id;
        return;
    }
    qDebug() << "Признаки изображения " << id << " удалены";
}

void DBWorker::changeImageId(int oldId, int newId)
{
    QStringList tables = {"Image", "Feature"};
    QStringList idColumns;
    for(int i = 0; i < tables.size(); ++i)
    {
        QString column = (i == 0) ? "id" : "image_id";
        idColumns.push_back(column);
    }

    QSqlQuery q(this->m_db);
    for(int i = 0; i < tables.size(); ++i)
    {
        q.prepare("UPDATE " + tables[i] + " SET " + idColumns[i] + " = ? WHERE " + idColumns[i] + " = ?");
        q.addBindValue(newId);
        q.addBindValue(oldId);
        if(q.exec())
            qDebug() << "Таблица " << tables[i] << ": id изображения изменен с " << oldId << " на " << newId;
        else
            qDebug() << "Таблица " << tables[i] << ": не удалось изменить id изображения с " << oldId << " на " << newId;
    }

}

QString DBWorker::sqlIdColumn(const QString &dataType)
{
    return "id " + dataType + " UNSIGNED UNIQUE PRIMARY KEY";
}

QByteArray DBWorker::imageToByteArray(const QString &imageUrl)
{
    QByteArray byteArray;

    if(!CppFunctions::imageIsFromProvider(imageUrl))
    {
        QString url(imageUrl);
        url.remove("file:///");
        QString ext;
        for(int ch = url.length() - 1;;--ch) // выделяем из строки url подстроку с форматом
        {
            if(url[ch] == '.') break;
            ext.push_front(url[ch]);
        }

        QImage image;
        if(!image.load(url))
        {
            qDebug() << "Ошибка при открытии изображения: " << url;
            return byteArray;
        }

        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        bool scs = image.save(&buffer, ext.toLocal8Bit().constData());
        qDebug() << "image [" << url << "], ext = " << ext << " : " << scs;
        if(!scs)
        {
            qDebug() << "Ошибка конвертирования изображения в QByteArray";
            return QByteArray();
        }
    }

    else
        byteArray = this->m_imageProvider->getByteArray(imageUrl);

    return byteArray;
}

int DBWorker::maxID(const QString &tableName)
{
    QSqlQuery q(this->m_db);
    q.prepare("SELECT MAX(id) FROM " + tableName);
    if(!q.exec())
    {
        qDebug() << "Таблица " << tableName << ": ошибка при нахождении максимального id";
        return -1;
    }
    q.next();
    return q.value(0).toInt();
}

int DBWorker::getIDForInsert(const QString &tableName)
{
    if(tableIsEmpty(tableName))
        return 1;
    return maxID(tableName) + 1;
}

int DBWorker::getIDByFieldData(const QString& tableName, const QString& fieldName, const QString& fieldData)
{
    QSqlQuery q(this->m_db);
    q.prepare("SELECT id FROM " + tableName + " WHERE (" + fieldName + " = ?)");
    q.addBindValue(fieldData);
    if(!q.exec())
    {
        qDebug() << "Ошибка при поиске ID по значению поля";
        return 0;
    }
    if(!q.next())
    {
        qDebug() << "getIDByFieldData: запись не найдена (" << tableName << ", " << fieldName << ": " << fieldData << ")";
        return 0;
    }
    return q.value(0).toInt();
}

QString DBWorker::getFieldDataByID(const QString& tableName, const QString& fieldName, int id)
{
    QSqlQuery q(this->m_db);
    QString idString;
    idString.setNum(id);
    q.prepare("SELECT " + fieldName + " FROM " + tableName + " WHERE (id = " + idString + ')');
    if(!q.exec())
    {
        qDebug() << "Ошибка при поиске значения поля по ID";
        return "";
    }
    if(!q.next())
    {
        qDebug() << "getFieldDataByID: запись не найдена (" << tableName << ", " << fieldName << ", id = " << id << ")";
        return "";
    }
    return q.value(0).toString();
}

void DBWorker::setImageProvider(ImageProvider *imageProvider)
{
    this->m_imageProvider = imageProvider;
}

int DBWorker::imagesCount()
{
    QSqlQuery q(this->m_db);
    q.prepare("SELECT COUNT(*) FROM Image");
    q.exec();
    q.next();
    return q.value(0).toInt();
}

QList<QObject*> DBWorker::allImagesModel(const QString &sort)
{
    QString order = "ORDER BY ";
    if(sort == "id") // default value
        order += "Image.id";
    else if(sort == "group")
        order += "\"Group\".name";
    else if(sort == "diagnosis")
        order += "Diagnosis.diagnosis";

    return this->imagesModel(order);

}

QObject *DBWorker::getImage(int id)
{
    QObject *image = new ImageModel(this);
    ImageModel *imageModel = qobject_cast<ImageModel*>(image);

    QString query = "SELECT Image.image, \"Group\".name, "
                    "Diagnosis.diagnosis FROM Image INNER JOIN "
                    "\"Group\" ON Image.group_id = \"Group\".id INNER JOIN "
                    "Diagnosis ON Image.diagnosis_id = Diagnosis.id WHERE Image.id = ?";
    QSqlQuery q(this->m_db);
    q.prepare(query);
    q.addBindValue(id);



    if(!q.exec())
    {
        imageModel->setError(true);
        qDebug() << "Ошибка получения изображения";
    }
    else
    {
        q.next();
        QByteArray byteArray = q.value(0).toByteArray();
        QString group = q.value(1).toString();
        QString diagnosis = q.value(2).toString();
        QImage img;
        img.loadFromData(byteArray);
        QString imageId = "image_1";
        this->m_imageProvider->clear();
        this->m_imageProvider->add(imageId, img, byteArray);
        QString source = "image://provider/" + imageId;

        imageModel->setData(id, source, group, diagnosis);

        // Features

        q.prepare("SELECT Feature.id, FeatureType.type, Feature.color FROM Feature "
                  "INNER JOIN FeatureType ON Feature.feature_type_id = FeatureType.id "
                  "WHERE Feature.image_id = ?");
        q.addBindValue(id);



        if(q.exec())
        {
            QList<QObject*> features;

            while(q.next())
            {
                int featureId = q.value(0).toInt();
                QString featureType = q.value(1).toString();
                QString color = q.value(2).toString();

                //qDebug() << color;

                QList<QObject*> markers = this->getMarkers(featureId);

                features.push_back(new FeatureModel(featureType, color, markers, this));
            }

            imageModel->setFeatures(features);
        }
        else
            qDebug() << "Ошибка при получении признаков";


    }



    return image;
}

QList<QObject *> DBWorker::getMarkers(int featureId)
{
//    columnsList.push_back(this->sqlIdColumn());
//    columnsList.push_back("feature_id INTEGER UNSIGNED");
//    columnsList.push_back("marker_type_id TINYINT UNSIGNED");
//    columnsList.push_back("line_width TINYINT UNSIGNED");
//    columnsList.push_back("x1 SMALLINT UNSIGNED");
//    columnsList.push_back("y1 SMALLINT UNSIGNED");
//    columnsList.push_back("x2 SMALLINT UNSIGNED");
//    columnsList.push_back("y2 SMALLINT UNSIGNED");

    QSqlQuery q(this->m_db);


    q.prepare("SELECT MarkerType.type, Marker.line_width, Marker.x1, Marker.y1, Marker.x2, Marker.y2 FROM "
              "Marker INNER JOIN MarkerType ON (Marker.marker_type_id = MarkerType.id) "
              "WHERE (Marker.feature_id = ?)");
    q.addBindValue(featureId);

    QList<QObject*> markers;

    if(!q.exec())
        qDebug() << "Ошибка получения маркеров";
    else
    {
        while(q.next())
        {
            QString type = q.value(0).toString();
            int lineWidth = q.value(1).toInt();
            int x1 = q.value(2).toInt();
            int y1 = q.value(3).toInt();
            int x2 = q.value(4).toInt();
            int y2 = q.value(5).toInt();


            markers.push_back(new MarkerModel(featureId, type, lineWidth, x1, y1, x2, y2, this));
        }
    }

    return markers;
}

QStringList DBWorker::distinctFeatures(int imageId)
{
    QStringList features;
    QSqlQuery q(this->m_db);
    q.prepare("SELECT type FROM FeatureType WHERE id IN (SELECT feature_type_id FROM Feature WHERE image_id = ?) ORDER BY id");
    q.addBindValue(imageId);
    if(!q.exec())
        qDebug() << "Ошибка при получении списка признаков изображения";
    else
        while(q.next())
            features.push_back(q.value(0).toString());
    return features;
}

void DBWorker::initTreeViewModel()
{
    this->m_treeViewModel->initModel(*this);
}

void DBWorker::clearImageProvider()
{
    this->m_imageProvider->clear();
}

QList<QObject *> DBWorker::findImagesByFilter(bool strict)
{
    // strict == true в режиме поиска типа "И"



    QStringList conditionsList;


    if(!this->m_treeViewModel->allChildrenAreChecked("Диагноз"))
    {
        /* Этот блок кода выполняется в следующих случаях:
         * 1) Не указано ничего в разделе "Диагноз" - тогда список checkedDiagnoses останется пустым
         * 2) Указано только "Меланома"
         * 3) Указано "Меланома" и частично "Не меланома"
         * 4) Указано только "Не меланома" (полностью или частично)
         */
        QStringList checkedDiagnoses;

        if(this->m_treeViewModel->elementIsChecked("Меланома"))
            checkedDiagnoses.push_back("Меланома");

        checkedDiagnoses.append(this->m_treeViewModel->getCheckedChildren("Не меланома"));


        if(!checkedDiagnoses.isEmpty())
        {
            QStringList checkedDiagnosesConditions;
            for(auto diagnosis: checkedDiagnoses)
            {
                int diagnosisId = this->getIDByFieldData("Diagnosis", "diagnosis", diagnosis);
                checkedDiagnosesConditions.push_back("Image.diagnosis_id = " + QString::number(diagnosisId));
            }

            QString diagnosesCondition = '(' + checkedDiagnosesConditions.join(" OR ") + ')';
            conditionsList.push_back(diagnosesCondition);
        }

    }

    if(this->m_treeViewModel->childrenAreChecked("Группа") && !this->m_treeViewModel->allChildrenAreChecked("Группа"))
    {
        QStringList checkedGroups = this->m_treeViewModel->getCheckedChildren("Группа");

        QStringList checkedGroupsConditions;
        for(auto group: checkedGroups)
        {
            int groupId = this->getIDByFieldData("\"Group\"", "name", group);
            checkedGroupsConditions.push_back("Image.group_id = " + QString::number(groupId));
        }
        QString groupsCondition = '(' + checkedGroupsConditions.join(" OR ") + ')';
        conditionsList.push_back(groupsCondition);
    }

    if(this->m_treeViewModel->childrenAreChecked("Признаки"))
    {
        QString featuresCondition;
        QStringList checkedFeatures = this->m_treeViewModel->getCheckedChildren("Признаки");
        QStringList featuresConditions;
        for(auto ch: checkedFeatures)
        {
            QString condition = "feature_type_id = ";
            condition += QString::number(this->getIDByFieldData("FeatureType", "type", ch));
            featuresConditions.push_back(condition);
        }

        if(!strict)  // Поиск типа "ИЛИ" - используем один SELECT с операторами "OR"
        {
            featuresCondition = "(Image.id IN (SELECT image_id FROM Feature WHERE " + featuresConditions.join(" OR ") + "))";
        }

        else /* Поиск "И" - делаем несколько SELECT'ов,
                затем ищем пересечение поученных множеств id изображений с помощью INTERSECT */
        {
            for(auto &cond: featuresConditions)
                cond = "SELECT image_id FROM Feature WHERE " + cond;

            featuresCondition = "(Image.id IN (" + featuresConditions.join(" INTERSECT ") + "))";
        }


        conditionsList.push_back(featuresCondition);

    }

    if(conditionsList.isEmpty()) // ничего не указано в фильтре => просто возвращаем все изображения
        return this->allImagesModel();

    // Если что-то указано в фильтре, то формируем одно общее условие для изображений
    QString generalCondition = "WHERE " + conditionsList.join(" AND ");

    qDebug () << generalCondition;
    return this->imagesModel(generalCondition);

}

QObject *DBWorker::execQuery(const QString &query)
{
    QObject *result = new QueryResultModel(this);

    QueryResultModel *queryResult = qobject_cast<QueryResultModel*>(result);

    QSqlQuery q(this->m_db);
    q.prepare(query);
    if(!q.exec())
        queryResult->setError(true);
    else
    {
        if(q.isSelect())
        {
            QSqlRecord record = q.record();
            int columnsCount = record.count();
            QStringList columnsNames;

            for(int i = 0; i < columnsCount; ++i)
                columnsNames << record.fieldName(i);

            queryResult->setTableHeader(columnsNames);
            QObjectList tableData;



            while(q.next())
            {
                QStringList rowData;
                for(int i = 0; i < columnsCount; ++i)
                {
                    QVariant value = q.value(i);

                    rowData << value.toString();
                }
                qDebug () << rowData;
                tableData.push_back(new TableRowModel(rowData, this));
            }

            queryResult->setTableData(tableData);
        }
    }

    return result;
}

void DBWorker::setTreeViewModel(TreeViewModel *treeViewModel)
{
    this->m_treeViewModel = treeViewModel;
}
