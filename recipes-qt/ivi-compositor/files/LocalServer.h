#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <functional>
#include <string>
#include <vector>
#include <pthread.h>

class LocalServer {
public:
    LocalServer();
    ~LocalServer();

    void startServer(const std::string &socketName);
    void sendData(const std::string &data, const std::string &socketName);

    // Callbacks
    void setBrightnessChangedCallback(const std::function<void(const std::string&)> &callback);
    void setOpcodeReceivedCallback(const std::function<void(const std::string&)> &callback);

private:
    int createUnixSocket(const std::string &socketName);
    void bindSocket(int socketFd, const std::string &socketName);
    void listenForConnections(int socketFd);
    void closeSocket(int &socketFd);

    void setupServer(const std::string &socketName);
    void monitorSockets();

    void handleNewConnection(int serverSocket);
    void handleClientData();
    void moveDataToClient(const std::string &clientName, const std::string &data);
    void processClientData(const std::string &data, const std::string &socketName);


    void handleClientDataForConnections(std::vector<int> &connections, pthread_mutex_t &mutex, fd_set &readfds);
    void handleClientDataForSocket(int clientSocket, std::vector<int> &connections);
    void handleClientDataThread(int clientSocket, std::vector<int> *connections, pthread_mutex_t *mutex);

    int m_socketBrightness;
    int m_socketOpcodeClient1;
    int m_socketOpcodeClient2;
    int m_socketMoveToClient1;
    int m_socketMoveToClient2;

    std::vector<int> m_clientConnectionsBrightness;
    std::vector<int> m_clientConnectionsOpcodeClient1;
    std::vector<int> m_clientConnectionsOpcodeClient2;
    std::vector<int> m_clientConnectionsMoveToClient1;
    std::vector<int> m_clientConnectionsMoveToClient2;

    pthread_mutex_t m_mutexBrightness;
    pthread_mutex_t m_mutexOpcodeClient1;
    pthread_mutex_t m_mutexOpcodeClient2;
    pthread_mutex_t m_mutexMoveToClient1;
    pthread_mutex_t m_mutexMoveToClient2;

    std::function<void(double)> m_brightnessChangedCallback;
    std::function<void(const std::string&)> m_opcodeDataReceivedCallback;
};

#endif // LOCALSERBER_H
