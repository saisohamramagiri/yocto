#include "localsocketwrapper.h"
#include <QByteArray>

LocalSocketWrapper::LocalSocketWrapper(QObject *parent)
    : QObject(parent) {
}

void LocalSocketWrapper::connectToServer(const QString &serverName) {
    if (socket.isOpen()) {
        socket.close();
    }

    socket.connectToServer(serverName);

    // Signal-Slot Connections:
    connect(&socket, &QLocalSocket::connected, this, &LocalSocketWrapper::connected);

    connect(&socket, &QLocalSocket::errorOccurred, this, [this]() {
        emit errorOccurred(socket.errorString());
    });
}

void LocalSocketWrapper::sendBrightnessValue(double value) {
    if (socket.isOpen()) {
        QByteArray data = QByteArray::number(value); // Convert value from double to QByteArray
        socket.write(data);
        socket.flush();
    }
}
