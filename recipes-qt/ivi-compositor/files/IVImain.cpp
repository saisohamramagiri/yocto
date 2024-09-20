#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <iostream>
#include "TcpServerHandler.h"
#include "LocalServer.h"

void setupServers(TcpServerHandler &tcpServerHandler, LocalServer *localServerBrightness,
                  LocalServer *localServerOpcodeClient1, LocalServer *localServerOpcodeClient2)
{
    // Start servers
    tcpServerHandler.startServer(51234);
    localServerBrightness->startServer("/tmp/brightnessSocket");
    localServerOpcodeClient1->startServer("/tmp/opcodeClient1Socket");
    localServerOpcodeClient2->startServer("/tmp/opcodeClient2Socket");

    static std::string currentSocketName = "/tmp/opcodeClient1Socket";
    static bool isSwitching = false;

    // Set the callback function for data received by the TCP server
    tcpServerHandler.setDataReceivedCallback([=](const std::string &dataString) mutable {
        std::cout << "Received opcode input from MFC: " << dataString << std::endl;
        if (dataString == "SWITCH_TO_CLIENT1") {
            currentSocketName = "/tmp/opcodeClient1Socket";
            isSwitching = false;
            std::cout << "Switched control to Client 1." << std::endl;
        } else if (dataString == "SWITCH_TO_CLIENT2") {
            currentSocketName = "/tmp/opcodeClient2Socket";
            isSwitching = false;
            std::cout << "Switched control to Client 2." << std::endl;
        } /*else if (dataString == "TABSPACE") {
            if (isSwitching) return; // Prevent multiple switches

            LocalServer *currentServer = (currentSocketName == "/tmp/opcodeClient1Socket") ? localServerOpcodeClient1 : localServerOpcodeClient2;
            LocalServer *nextServer = (currentSocketName == "/tmp/opcodeClient1Socket") ? localServerOpcodeClient2 : localServerOpcodeClient1;
            std::string nextSocketName = (currentSocketName == "/tmp/opcodeClient1Socket") ? "/tmp/opcodeClient2Socket" : "/tmp/opcodeClient1Socket";

            std::cout << "Sending TABSPACE to current server: " << currentSocketName << std::endl;
            currentServer->sendData("TABSPACE", currentSocketName);

            // Use a lambda to handle data received from the current server
            auto handleClientDataForConnections = [=](const std::string &response) mutable {
                if (response == "NOT CONSUMED") {
                    std::cout << "Switching to " << (currentSocketName == "/tmp/opcodeClient1Socket" ? "Client 2" : "Client 1") << std::endl;
                    nextServer->sendData("TABSPACE", nextSocketName);
                    currentSocketName = nextSocketName;
                    std::cout << "Control now with " << (currentSocketName == "/tmp/opcodeClient1Socket" ? "Client 1" : "Client 2") << std::endl;

                }  else {
                    std::cout << "Response from current server was consumed. No switch performed." << std::endl;
                }
                isSwitching = false;
                std::cout << "Resetting isSwitching flag to false." << std::endl;
            };

            currentServer->setOpcodeReceivedCallback(handleClientDataForConnections);

            isSwitching = true;
            std::cout << "Set isSwitching flag to true." << std::endl;

        } */else {
            // Forward opcode data to the current client
            LocalServer *currentServer = (currentSocketName == "/tmp/opcodeClient1Socket") ? localServerOpcodeClient1 : localServerOpcodeClient2;
            currentServer->sendData(dataString, currentSocketName);
            std::cout << "Forwarded opcode data to local socket: " << dataString << std::endl;
        }
    });

    // // Handle brightness changes
    // auto handleBrightnessChange = [](double value) {
    //     std::cout << "Received brightness value: " << value << std::endl;
    // };

    // localServerBrightness->setBrightnessChangedCallback(handleBrightnessChange);
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    TcpServerHandler tcpServerHandler;
    LocalServer localServerBrightness;
    LocalServer localServerOpcodeClient1;
    LocalServer localServerOpcodeClient2;

    qmlRegisterType<TcpServerHandler>("com.example", 1, 0, "TcpServerHandler");
    qmlRegisterType<LocalServer>("com.example", 1, 0, "LocalServer");

    //engine.rootContext()->setContextProperty("localServerBrightness", &localServerBrightness);
    //engine.rootContext()->setContextProperty("localServerOpcodeClient1", &localServerOpcodeClient1);
    //engine.rootContext()->setContextProperty("localServerOpcodeClient2", &localServerOpcodeClient2);

    setupServers(tcpServerHandler, &localServerBrightness, &localServerOpcodeClient1, &localServerOpcodeClient2);

    engine.load(QUrl::fromLocalFile("IVImain.qml"));

    return app.exec();
}
