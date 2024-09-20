#ifndef CLIENT2LOCALSOCKETWRAPPER_H
#define CLIENT2LOCALSOCKETWRAPPER_H

#include <QObject>
#include <QString>
#include <thread>

class Client2LocalSocketWrapper : public QObject
{
    Q_OBJECT

public:
    explicit Client2LocalSocketWrapper(QObject *parent = nullptr);
    ~Client2LocalSocketWrapper();

    Q_INVOKABLE void connectToServer(const QString &path);
    Q_INVOKABLE void sendData(const QString &data);

signals:
    void dataReceived(const QString &data);

private:
    void startPolling();
    void handleReadyRead();
    void handleError(const std::string &errorString);

    int m_socket;
    std::thread m_pollingThread;  // Core C++ thread for polling
    bool m_stopPolling;          // Flag to stop the polling thread
};

#endif // CLIENT2LOCALSOCKETWRAPPER_H
