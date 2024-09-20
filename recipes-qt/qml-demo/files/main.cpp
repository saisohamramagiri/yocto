#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    qputenv("QT_IVI_SURFACE_ID", "2222");
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl::fromLocalFile("Main.qml"));
//    const QUrl url(u"qrc:/qt-qml-demo/Main.qml"_qs);
//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//        &app, [url](QObject *obj, const QUrl &objUrl) {
//            if (!obj && url == objUrl)
//                QCoreApplication::exit(-1);
//        }, Qt::QueuedConnection);
//    engine.load(url);

    return app.exec();
}
