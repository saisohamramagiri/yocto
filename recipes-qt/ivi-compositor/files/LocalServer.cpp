#include "LocalServer.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <thread>
#include <algorithm>
#include <fcntl.h>
#include <errno.h>
#include <vector>

LocalServer::LocalServer() :
    m_socketBrightness(-1),
    m_socketOpcodeClient1(-1),
    m_socketOpcodeClient2(-1),
    m_socketMoveToClient1(-1),
    m_socketMoveToClient2(-1)
{
    pthread_mutex_init(&m_mutexBrightness, nullptr);
    pthread_mutex_init(&m_mutexOpcodeClient1, nullptr);
    pthread_mutex_init(&m_mutexOpcodeClient2, nullptr);
    pthread_mutex_init(&m_mutexMoveToClient1, nullptr);
    pthread_mutex_init(&m_mutexMoveToClient2, nullptr);

    std::cout << "LocalServer created." << std::endl;
}

LocalServer::~LocalServer() {
    closeSocket(m_socketBrightness);
    closeSocket(m_socketOpcodeClient1);
    closeSocket(m_socketOpcodeClient2);
    closeSocket(m_socketMoveToClient1);
    closeSocket(m_socketMoveToClient2);

    for (int fd : m_clientConnectionsBrightness) {
        closeSocket(fd);
    }
    for (int fd : m_clientConnectionsOpcodeClient1) {
        closeSocket(fd);
    }
    for (int fd : m_clientConnectionsOpcodeClient2) {
        closeSocket(fd);
    }
    for (int fd : m_clientConnectionsMoveToClient1) {
        closeSocket(fd);
    }
    for (int fd : m_clientConnectionsMoveToClient2) {
        closeSocket(fd);
    }

    pthread_mutex_destroy(&m_mutexBrightness);
    pthread_mutex_destroy(&m_mutexOpcodeClient1);
    pthread_mutex_destroy(&m_mutexOpcodeClient2);
    pthread_mutex_destroy(&m_mutexMoveToClient1);
    pthread_mutex_destroy(&m_mutexMoveToClient2);
}

int LocalServer::createUnixSocket(const std::string &socketName) {
    int sockFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockFd < 0) {
        std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "Created Unix socket with file descriptor: " << sockFd << std::endl;
    return sockFd;
}

void LocalServer::bindSocket(int socketFd, const std::string &socketName) {
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketName.c_str(), sizeof(addr.sun_path) - 1);

    unlink(socketName.c_str());

    if (bind(socketFd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) < 0) {
        std::cerr << "Socket binding failed for " << socketName << ": " << strerror(errno) << std::endl;
        close(socketFd);
    } else {
        std::cout << "Socket bound successfully to " << socketName << std::endl;
    }
}

void LocalServer::listenForConnections(int socketFd) {
    if (listen(socketFd, 5) < 0) {
        std::cerr << "Socket listen failed: " << strerror(errno) << std::endl;
        close(socketFd);
    } else {
        std::cout << "Listening for connections on socket: " << socketFd << std::endl;
    }
}

void LocalServer::startServer(const std::string &socketName) {
    std::cout << "Starting server with socket name: " << socketName << std::endl;
    setupServer(socketName);

    std::thread([this]() {
        monitorSockets();
    }).detach();
}

void LocalServer::setupServer(const std::string &socketName) {
    int *socketFd = nullptr;
    pthread_mutex_t *mutex = nullptr;
    std::vector<int> *connections = nullptr;

    if (socketName.find("brightness") != std::string::npos) {
        socketFd = &m_socketBrightness;
        mutex = &m_mutexBrightness;
        connections = &m_clientConnectionsBrightness;
    } else if (socketName.find("opcodeClient1") != std::string::npos) {
        socketFd = &m_socketOpcodeClient1;
        mutex = &m_mutexOpcodeClient1;
        connections = &m_clientConnectionsOpcodeClient1;
    } else if (socketName.find("opcodeClient2") != std::string::npos) {
        socketFd = &m_socketOpcodeClient2;
        mutex = &m_mutexOpcodeClient2;
        connections = &m_clientConnectionsOpcodeClient2;
    } else if (socketName.find("moveToClient1") != std::string::npos) {
        socketFd = &m_socketMoveToClient1;
        mutex = &m_mutexMoveToClient1;
        connections = &m_clientConnectionsMoveToClient1;
    } else if (socketName.find("moveToClient2") != std::string::npos) {
        socketFd = &m_socketMoveToClient2;
        mutex = &m_mutexMoveToClient2;
        connections = &m_clientConnectionsMoveToClient2;
    } else {
        std::cerr << "Unknown server type: " << socketName << std::endl;
        return;
    }

    closeSocket(*socketFd);
    *socketFd = createUnixSocket(socketName);
    if (*socketFd == -1) {
        std::cerr << "Failed to create socket for " << socketName << std::endl;
        return;
    }

    std::cout << "Socket created successfully for " << socketName << std::endl;

    bindSocket(*socketFd, socketName);
    listenForConnections(*socketFd);
}

void LocalServer::monitorSockets() {
    while (true) {
        fd_set readfds;
        FD_ZERO(&readfds);

        int maxFd = -1;

        if (m_socketBrightness != -1) {
            FD_SET(m_socketBrightness, &readfds);
            maxFd = std::max(maxFd, m_socketBrightness);
        }
        if (m_socketOpcodeClient1 != -1) {
            FD_SET(m_socketOpcodeClient1, &readfds);
            maxFd = std::max(maxFd, m_socketOpcodeClient1);
        }
        if (m_socketOpcodeClient2 != -1) {
            FD_SET(m_socketOpcodeClient2, &readfds);
            maxFd = std::max(maxFd, m_socketOpcodeClient2);
        }
        if (m_socketMoveToClient1 != -1) {
            FD_SET(m_socketMoveToClient1, &readfds);
            maxFd = std::max(maxFd, m_socketMoveToClient1);
        }
        if (m_socketMoveToClient2 != -1) {
            FD_SET(m_socketMoveToClient2, &readfds);
            maxFd = std::max(maxFd, m_socketMoveToClient2);
        }

        int activity = select(maxFd + 1, &readfds, nullptr, nullptr, nullptr);

        if (activity < 0) {
            std::cerr << "Select error: " << strerror(errno) << std::endl;
            continue;
        }

        if (m_socketBrightness != -1 && FD_ISSET(m_socketBrightness, &readfds)) {
            handleNewConnection(m_socketBrightness);
        }
        if (m_socketOpcodeClient1 != -1 && FD_ISSET(m_socketOpcodeClient1, &readfds)) {
            handleNewConnection(m_socketOpcodeClient1);
        }
        if (m_socketOpcodeClient2 != -1 && FD_ISSET(m_socketOpcodeClient2, &readfds)) {
            handleNewConnection(m_socketOpcodeClient2);
        }
        if (m_socketMoveToClient1 != -1 && FD_ISSET(m_socketMoveToClient1, &readfds)) {
            handleNewConnection(m_socketMoveToClient1);
        }
        if (m_socketMoveToClient2 != -1 && FD_ISSET(m_socketMoveToClient2, &readfds)) {
            handleNewConnection(m_socketMoveToClient2);
        }

        handleClientData();
    }
}

void LocalServer::handleNewConnection(int serverSocket) {
    int newClient = accept(serverSocket, nullptr, nullptr);
    if (newClient >= 0) {
        std::cout << "New client connected on socket: " << serverSocket << " Client FD: " << newClient << std::endl;

        pthread_mutex_t *mutex = nullptr;
        std::vector<int> *connections = nullptr;

        if (serverSocket == m_socketBrightness) {
            mutex = &m_mutexBrightness;
            connections = &m_clientConnectionsBrightness;
        } else if (serverSocket == m_socketOpcodeClient1) {
            mutex = &m_mutexOpcodeClient1;
            connections = &m_clientConnectionsOpcodeClient1;
        } else if (serverSocket == m_socketOpcodeClient2) {
            mutex = &m_mutexOpcodeClient2;
            connections = &m_clientConnectionsOpcodeClient2;
        } else if (serverSocket == m_socketMoveToClient1) {
            mutex = &m_mutexMoveToClient1;
            connections = &m_clientConnectionsMoveToClient1;
        } else if (serverSocket == m_socketMoveToClient2) {
            mutex = &m_mutexMoveToClient2;
            connections = &m_clientConnectionsMoveToClient2;
        }

        if (mutex && connections) {
            pthread_mutex_lock(mutex);
            connections->push_back(newClient);
            pthread_mutex_unlock(mutex);
        }
    } else {
        std::cerr << "Accept failed: " << strerror(errno) << std::endl;
    }
}

void LocalServer::handleClientData() {
    std::vector<int> sockets = { m_socketBrightness, m_socketOpcodeClient1, m_socketOpcodeClient2, m_socketMoveToClient1, m_socketMoveToClient2 };

    for (int socketFd : sockets) {
        if (socketFd == -1) continue;

        std::vector<int> *connections = nullptr;
        pthread_mutex_t *mutex = nullptr;

        if (socketFd == m_socketBrightness) {
            mutex = &m_mutexBrightness;
            connections = &m_clientConnectionsBrightness;
        } else if (socketFd == m_socketOpcodeClient1) {
            mutex = &m_mutexOpcodeClient1;
            connections = &m_clientConnectionsOpcodeClient1;
        } else if (socketFd == m_socketOpcodeClient2) {
            mutex = &m_mutexOpcodeClient2;
            connections = &m_clientConnectionsOpcodeClient2;
        } else if (socketFd == m_socketMoveToClient1) {
            mutex = &m_mutexMoveToClient1;
            connections = &m_clientConnectionsMoveToClient1;
        } else if (socketFd == m_socketMoveToClient2) {
            mutex = &m_mutexMoveToClient2;
            connections = &m_clientConnectionsMoveToClient2;
        }

        if (connections && mutex) {
            pthread_mutex_lock(mutex);

            for (size_t i = 0; i < connections->size(); ++i) {
                char buffer[256];
                ssize_t bytesRead = recv((*connections)[i], buffer, sizeof(buffer), MSG_DONTWAIT);
                if (bytesRead > 0) {
                    buffer[bytesRead] = '\0';
                    std::string data(buffer);

                    std::cout << "Opcode received: " << data << " on socket FD: " << (*connections)[i] << std::endl;


                    // Handle the received data
                    if (data.find("SWITCH_TO_CLIENT1") != std::string::npos) {
                        moveDataToClient("Client1", data);
                    } else if (data.find("SWITCH_TO_CLIENT2") != std::string::npos) {
                        moveDataToClient("Client2", data);
                    } else {
                        std::cerr << "Unknown command received: " << data << std::endl;
                    }
                } else if (bytesRead == 0) {
                    closeSocket((*connections)[i]);
                    connections->erase(connections->begin() + i);
                    --i;
                } else {
                    std::cerr << "Receive error: " << strerror(errno) << std::endl;
                }
            }

            pthread_mutex_unlock(mutex);
        }
    }
}

void LocalServer::moveDataToClient(const std::string &clientName, const std::string &data) {
    int *socketFd = nullptr;
    pthread_mutex_t *mutex = nullptr;
    std::vector<int> *connections = nullptr;

    if (clientName == "Client1") {
        socketFd = &m_socketOpcodeClient1;
        mutex = &m_mutexOpcodeClient1;
        connections = &m_clientConnectionsOpcodeClient1;
    } else if (clientName == "Client2") {
        socketFd = &m_socketOpcodeClient2;
        mutex = &m_mutexOpcodeClient2;
        connections = &m_clientConnectionsOpcodeClient2;
    } else {
        std::cerr << "Unknown client: " << clientName << std::endl;
        return;
    }

    if (socketFd && mutex && connections) {
        pthread_mutex_lock(mutex);

        for (int connection : *connections) {
            if (send(connection, data.c_str(), data.size(), 0) < 0) {
                std::cerr << "Send error to " << clientName << ": " << strerror(errno) << std::endl;
            }
        }

        pthread_mutex_unlock(mutex);
    }
}

// void LocalServer::setBrightnessChangedCallback(const std::function<void(const std::string&)> &callback) {
//     m_brightnessChangedCallback = callback;
// }

void LocalServer::setOpcodeReceivedCallback(const std::function<void(const std::string&)> &callback) {
    m_opcodeDataReceivedCallback = callback;
}

void LocalServer::processClientData(const std::string &data, const std::string &socketName) {
/*    if (socketName.find("brightness") != std::string::npos && m_brightnessChangedCallback) {
        m_brightnessChangedCallback(data);
    } else */if ((socketName.find("opcodeClient1") != std::string::npos ||
                socketName.find("opcodeClient2") != std::string::npos) && m_opcodeDataReceivedCallback) {
        m_opcodeDataReceivedCallback(data);
    }
}

void LocalServer::sendData(const std::string &data, const std::string &socketName) {
    std::vector<int> *connections = nullptr;

    if (socketName == "/tmp/brightnessSocket") {
        connections = &m_clientConnectionsBrightness;
    } else if (socketName == "/tmp/opcodeClient1Socket") {
        connections = &m_clientConnectionsOpcodeClient1;
    } else if (socketName == "/tmp/opcodeClient2Socket") {
        connections = &m_clientConnectionsOpcodeClient2;
    } else if (socketName == "/tmp/moveToClient1Socket") {
        connections = &m_clientConnectionsMoveToClient1;
    } else if (socketName == "/tmp/moveToClient2Socket") {
        connections = &m_clientConnectionsMoveToClient2;
    } else {
        std::cerr << "Unknown socket name: " << socketName << std::endl;
        return;
    }

    if (connections && !connections->empty()) {
        for (int clientSocket : *connections) {
            ssize_t bytesSent = write(clientSocket, data.data(), data.size());
            if (bytesSent < 0) {
                std::cerr << "Failed to send data on socket: " << strerror(errno) << std::endl;
            } else {
                std::cout << "Data sent to client on " << socketName << ": " << data << std::endl;
            }
        }
    } else {
        std::cerr << "No clients connected to socket: " << socketName << std::endl;
    }
}


void LocalServer::closeSocket(int &socketFd) {
    if (socketFd != -1) {
        close(socketFd);
        socketFd = -1;
    }
}
