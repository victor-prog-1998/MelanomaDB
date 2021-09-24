#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQmlContext>
#include <QIcon>
#include "dbworker.h"
#include "painteditem.h"
#include "treeviewmodel.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/icons/application-icon.png"));
    QCoreApplication::setApplicationName("База меланом");

    DBWorker dbWorker;
    TreeViewModel treeViewModel;
    // treeViewModel.initModel(dbWorker);
    dbWorker.setTreeViewModel(&treeViewModel);

    ImageProvider imageProvider;

    dbWorker.setImageProvider(&imageProvider);

    QQmlApplicationEngine engine;

    qmlRegisterType<PaintedItem>("com.PaintedItem", 1, 0, "PaintedItem");


    engine.rootContext()->setContextProperty("dbWorker", &dbWorker);
    engine.rootContext()->setContextProperty("treeViewModel", &treeViewModel);


    engine.addImageProvider("provider", &imageProvider);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
