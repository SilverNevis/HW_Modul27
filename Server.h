#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <string>

class Server {
private:
    SOCKET serverSocket;
    fd_set master;

public:
    Server();
    ~Server();
    void run();
    void sendToClient(int clientSocket, const std::string& msg);
    void broadcast(const std::string& msg);
    void removeClient(int clientSocket);
};

#endif
