#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "LocalSocketWrapperNew.h"

int main(int argc, char *argv[])
{
    qputenv("QT_IVI_SURFACE_ID", "1111");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    LocalSocketWrapperNew LocalSocketWrapperNew;
    engine.rootContext()->setContextProperty("LocalSocketWrapperNew", &LocalSocketWrapperNew);

    // QObject::connect(
    //     &engine,
    //     &QQmlApplicationEngine::objectCreationFailed,
    //     &app,
    //     []() { QCoreApplication::exit(-1); },
    //     Qt::QueuedConnection);
    engine.loadFromModule("qml-tab-space-func", "TABMain");

    return app.exec();
}
