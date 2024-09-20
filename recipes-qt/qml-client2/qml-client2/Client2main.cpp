#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "client2localsocketwrapper.h"

int main(int argc, char *argv[])
{
    qputenv("QT_IVI_SURFACE_ID", "2222");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    Client2LocalSocketWrapper client2localsocketwrapper;
    engine.rootContext()->setContextProperty("client2localsocketwrapper", &client2localsocketwrapper);
    //qmlRegisterType<Client2LocalSocketWrapper>("com.yourcompany", 1, 0, "client2localsocketwrapper");


    qmlRegisterType<Client2LocalSocketWrapper>("com.yourcompany", 1, 0, "Client2LocalSocketWrapper");


    // QObject::connect(
    //     &engine,
    //     &QQmlApplicationEngine::objectCreationFailed,
    //     &app,
    //     []() { QCoreApplication::exit(-1); },
    //     Qt::QueuedConnection);
    engine.loadFromModule("qml-client2", "Client2Main");

    return app.exec();
}
