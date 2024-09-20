#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "localsocketwrapper.h"

int main(int argc, char *argv[])
{
    qputenv("QT_IVI_SURFACE_ID", "3333");
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    LocalSocketWrapper localSocketWrapper;
    engine.rootContext()->setContextProperty("localSocketWrapper", &localSocketWrapper);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("qml-brightness-socket", "BrightnessMain");

    return app.exec();
}
