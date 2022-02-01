#ifndef DBWORKER_H
#define DBWORKER_H

#include <QObject>
#include <QSqlDatabase>

#include "imageprovider.h"
#include "painteditem.h"
#include "treeviewmodel.h"

class DBWorker : public QObject
{
    Q_OBJECT
public:
    explicit DBWorker(QObject *parent = nullptr);
    virtual ~DBWorker() {}


    void setTreeViewModel(TreeViewModel *treeViewModel);

    void createTables();
    bool createTable(const QString& title, const QString& columns);
    bool createTable(const QString& title, const QStringList& columns);

    bool tableIsEmpty(const QString &tableName);
    int maxID(const QString& tableName);
    int getIDForInsert(const QString& tableName);
    int getIDByFieldData(const QString& tableName, const QString& fieldName, const QString& fieldData);
    QString getFieldDataByID(const QString& tableName, const QString& fieldName, int id);


    // Reference tables
    void fillReferenceTables();
    void fillDiagnosisTable();
    void fillGroupTable();
    void fillFeatureTypeTable();
    void fillMarkerTypeTable();

    // Списки строк

    Q_INVOKABLE QStringList nonMelanomaDiagnoses();
    Q_INVOKABLE QStringList groups();
    Q_INVOKABLE QStringList featureTypes();
    Q_INVOKABLE QStringList markerTypes();


    // Добавление / удаление записей


    // originalId задается при редактировании. В таблице Image (и остальных, куда пишется информация об изображении)
    // создается новая запись (с новым id изображения). Если добавление прошло успешно во все эти таблицы,
    // то все исходные записи удаляются, а затем в новой записи новый id меняется на исходный.
    // В методе insertImage originalId нужен, чтобы получить из БД дату и время добавления изображения

    Q_INVOKABLE int insertImage(const QString& group, const QString& diagnosis,
                                const QString& imageUrl, int originalId = 0);

    Q_INVOKABLE int insertFeature(int imageId, const QString& type, const QString& color);
    Q_INVOKABLE void insertMarker(int featureId, const QString& type, int lineWidth, int x1, int y1, int x2, int y2);


    Q_INVOKABLE void deleteImage(int id);
    Q_INVOKABLE void changeImageId(int oldId, int newId);

    int insertNewDiagnosis(const QString& diagnosis);
    int insertNewGroup(const QString& group);
    bool deleteRecords(const QString &tableName, const QString &conditionField, const QVariant &value);


    /* Возвращает модель изображений
     * В строке conditions могут указываться условия поиска либо указания для сортировки
     * Если строка не задается, то строится модель всех изображений
     */
    QList<QObject *> imagesModel(const QString& conditions = "");

    void setImageProvider(ImageProvider *imageProvider);


    Q_INVOKABLE int imagesCount();

    Q_INVOKABLE QList<QObject*> allImagesModel(const QString& sort = "id");

    Q_INVOKABLE QObject* getImage(int id);


    Q_INVOKABLE void clearImageProvider();

    Q_INVOKABLE QList<QObject*> findImagesByFilter(bool strict);


    Q_INVOKABLE QObject* execQuery(const QString& query);


    QList<QObject *> getMarkers(int featureId);

    QStringList distinctFeatures(int imageId);

    Q_INVOKABLE void initTreeViewModel();



private:
    QSqlDatabase m_db;
    QString sqlIdColumn(const QString& dataType = "INTEGER"); // чтобы не писать "id INTEGER UNSIGNED UNIQUE PRIMARY KEY"
    // возвращает конвертированное в QByteArray изображение, находящееся по ссылке imageUrl
    QByteArray imageToByteArray(const QString& imageUrl);

    ImageProvider *m_imageProvider;
    TreeViewModel *m_treeViewModel;


signals:

public slots:
};

#endif // DBWORKER_H
