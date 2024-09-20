#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include <QtQml/qqmlregistration.h>
#include "windowsan.h"
int main(int argc, char *argv[])
{
    qputenv("QT_IVI_SURFACE_ID", "1111");
    qmlRegisterType<Windowsan>("Windowsan", 1, 0, "Windowsan");

    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.load(QUrl::fromLocalFile("Main2.qml"));
//    const QUrl url(u"qrc:/qt-qml-demo/Main2.qml"_qs);
//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//        &app, [url](QObject *obj, const QUrl &objUrl) {
//            if (!obj && url == objUrl)
//                QCoreApplication::exit(-1);
//        }, Qt::QueuedConnection);
//    engine.load(url);

    return app.exec();
}
