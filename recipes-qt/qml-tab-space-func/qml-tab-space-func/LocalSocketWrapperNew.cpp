#include "LocalSocketWrapperNew.h"
#include <iostream>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>

LocalSocketWrapperNew::LocalSocketWrapperNew(QObject *parent)
    : QObject(parent), m_socket(-1), m_stopPolling(false)
{
}

LocalSocketWrapperNew::~LocalSocketWrapperNew()
{
    m_stopPolling = true;
    if (m_pollingThread.joinable()) {
        m_pollingThread.join();
    }
    if (m_socket >= 0) {
        close(m_socket);
    }
}

void LocalSocketWrapperNew::connectToServer(const QString &path)
{
    std::string pathStr = path.toStdString(); // Convert QString to std::string

    if (m_socket >= 0) {
        close(m_socket);
    }

    m_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (m_socket < 0) {
        handleError("Failed to create socket");
        return;
    }

    sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, pathStr.c_str(), sizeof(addr.sun_path) - 1);

    if (::connect(m_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        handleError("Failed to connect to socket");
        close(m_socket);
        m_socket = -1;
        return;
    }

    std::cout << "Connected to local socket at path: " << pathStr << std::endl;

    // Set the socket to non-blocking mode
    int flags = fcntl(m_socket, F_GETFL, 0);
    if (flags == -1) {
        handleError("Failed to get socket flags");
        close(m_socket);
        m_socket = -1;
        return;
    }

    if (fcntl(m_socket, F_SETFL, flags | O_NONBLOCK) < 0) {
        handleError("Failed to set socket to non-blocking mode");
        close(m_socket);
        m_socket = -1;
        return;
    }

    // Start polling in a separate thread
    m_stopPolling = false;
    m_pollingThread = std::thread(&LocalSocketWrapperNew::startPolling, this);
}

void LocalSocketWrapperNew::startPolling()
{
    while (!m_stopPolling && m_socket >= 0) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(m_socket, &readfds);

        int result = select(m_socket + 1, &readfds, nullptr, nullptr, nullptr);
        if (result > 0) {
            if (FD_ISSET(m_socket, &readfds)) {
                handleReadyRead();
            }
        } else if (result < 0) {
            handleError("select() failed");
            break;
        }
    }
}

void LocalSocketWrapperNew::handleReadyRead()
{
    char buffer[1024];
    ssize_t bytesRead = read(m_socket, buffer, sizeof(buffer) - 1);

    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::string opcode(buffer);
        opcode.erase(opcode.find_last_not_of(" \n\r\t") + 1); // Trim whitespace
        std::cout << "Opcode received from local socket: " << opcode << std::endl;
        emit dataReceived(QString::fromStdString(opcode)); // Convert std::string to QString
    } else if (bytesRead < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
        handleError("Failed to read from socket");
    }
}

void LocalSocketWrapperNew::sendData(const QString &data)
{
    if (m_socket < 0) {
        handleError("Cannot send data. Not connected to socket.");
        return;
    }

    std::string dataToSend = data.toStdString() + '\n'; // Convert QString to std::string and add newline
    ssize_t bytesWritten = write(m_socket, dataToSend.c_str(), dataToSend.size());
    if (bytesWritten < 0) {
        handleError("Failed to write data to socket");
    } else {
        std::cout << "Data sent to local socket: " << dataToSend << std::endl;
    }
}

void LocalSocketWrapperNew::handleError(const std::string &errorString)
{
    std::cerr << errorString << ": " << strerror(errno) << std::endl;
}
