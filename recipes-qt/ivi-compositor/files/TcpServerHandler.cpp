#include "TcpServerHandler.h"
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <thread>

TcpServerHandler::TcpServerHandler()
    : m_tcpServerSocket(-1),
    m_localServerSocket(-1)
{
}

TcpServerHandler::~TcpServerHandler()
{
    closeSocket(m_tcpServerSocket);
    closeSocket(m_localServerSocket);
    for (int clientSocket : m_clientConnections) {
        closeSocket(clientSocket);
    }
}

void TcpServerHandler::startServer(uint16_t port)
{
    m_tcpServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_tcpServerSocket < 0) {
        std::cerr << "Failed to create TCP socket: " << strerror(errno) << std::endl;
        return;
    }

    sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(m_tcpServerSocket, reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to bind TCP socket: " << strerror(errno) << std::endl;
        closeSocket(m_tcpServerSocket);
        return;
    }

    if (listen(m_tcpServerSocket, 5) < 0) {
        std::cerr << "Failed to listen on TCP socket: " << strerror(errno) << std::endl;
        closeSocket(m_tcpServerSocket);
        return;
    }

    std::cout << "TCP Server started on port " << port << std::endl;

    // Start a thread to accept connections
    std::thread([this]() {
        while (true) {
            acceptConnection();
        }
    }).detach(); // Detach the thread so it runs independently
}


void TcpServerHandler::acceptConnection()
{
    int clientSocket = accept(m_tcpServerSocket, nullptr, nullptr);
    if (clientSocket >= 0) {
        std::cout << "Client connected, client socket: " << clientSocket << std::endl;

        // Set the client socket to non-blocking mode
        if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) < 0) {
            std::cerr << "Failed to set non-blocking mode: " << strerror(errno) << std::endl;
            closeSocket(clientSocket);
            return;
        }

        m_clientConnections.push_back(clientSocket);

        // Handle client data in a separate thread
        std::thread(&TcpServerHandler::handleClientData, this, clientSocket).detach();
    } else {
        std::cerr << "Failed to accept connection: " << strerror(errno) << std::endl;
    }
}

void TcpServerHandler::handleClientData(int clientSocket) {
    while (true) {
        char buffer[4096];
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::string data(buffer);

            // Call the callback function if it's set
            if (m_dataReceivedCallback) {
                m_dataReceivedCallback(data);
            }

            // Forward data to the local server if connected
            if (m_localServerSocket >= 0) {
                send(m_localServerSocket, data.c_str(), data.size(), 0);
            }
        } else if (bytesRead == 0) {
            closeSocket(clientSocket);
            break; // Exit the loop when the client disconnects
        } else if (bytesRead < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            closeSocket(clientSocket);
            break; // Exit the loop on an error other than EAGAIN or EWOULDBLOCK
        }

        // Sleep briefly to prevent busy-waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


void TcpServerHandler::connectToLocalServer(const std::string &socketName)
{
    m_localServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_localServerSocket < 0) {
        std::cerr << "Failed to create local TCP socket: " << strerror(errno) << std::endl;
        return;
    }

    sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(12345);

    if (::connect(m_localServerSocket, reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to connect to local server: " << strerror(errno) << std::endl;
        closeSocket(m_localServerSocket);
    } else {
        std::cout << "Connected to local server" << std::endl;
    }
}

void TcpServerHandler::closeSocket(int socket)
{
    if (socket >= 0) {
        close(socket);
    }
}

void TcpServerHandler::setDataReceivedCallback(const std::function<void(const std::string&)>& callback) {
    m_dataReceivedCallback = callback; // Store the callback function
}
