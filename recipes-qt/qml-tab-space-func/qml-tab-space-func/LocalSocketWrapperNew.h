#ifndef LOCALSOCKETWRAPPERNEW_H
#define LOCALSOCKETWRAPPERNEW_H

#include <QObject>
#include <QString>
#include <thread>

class LocalSocketWrapperNew : public QObject
{
    Q_OBJECT

public:
    explicit LocalSocketWrapperNew(QObject *parent = nullptr);
    ~LocalSocketWrapperNew();

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

#endif // LocalSocketWrapperNew_H
