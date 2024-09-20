#ifndef TCPSERVERHANDLER_H
#define TCPSERVERHANDLER_H

#include <vector>
#include <string>
#include <functional>

class TcpServerHandler {
public:
    TcpServerHandler();  // Constructor to initialize the server handler
    ~TcpServerHandler(); // Destructor to clean up resources

    void startServer(uint16_t port); // Function to start the TCP server on a specified port
    void connectToLocalServer(const std::string &socketName); // Connect to a local server by its name

    void setDataReceivedCallback(const std::function<void(const std::string&)>& callback); // Set the callback function for received data

private:
    int m_tcpServerSocket;           // Server socket for accepting connections
    int m_localServerSocket;         // Socket to connect to a local server
    std::vector<int> m_clientConnections; // List of active client connections

    std::function<void(const std::string&)> m_dataReceivedCallback; // Callback for handling received data

    void acceptConnection();         // Accept a new client connection
    void handleClientData(int clientSocket); // Handle incoming data from a client
    void closeSocket(int socket);    // Close a specified socket
};

#endif // TCPSERVERHANDLER_H
