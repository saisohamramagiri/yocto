#ifndef LOCALSOCKETWRAPPER_H
#define LOCALSOCKETWRAPPER_H

#include <QObject>
#include <QLocalSocket>

class LocalSocketWrapper : public QObject {
    Q_OBJECT
public:
    explicit LocalSocketWrapper(QObject *parent = nullptr);

    // Method to connect to the server
    Q_INVOKABLE void connectToServer(const QString &serverName);

    // Method to send brightness value
    Q_INVOKABLE void sendBrightnessValue(double value);

signals:
    // Signal emitted when connected to the server
    void connected();

    // Signal emitted when an error occurs
    void errorOccurred(const QString &error);

private:
    QLocalSocket socket; // Local socket for communication
};

#endif // LOCALSOCKETWRAPPER_H
