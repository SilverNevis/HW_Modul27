#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <string>

class Client {
private:
    SOCKET clientSocket;

public:
    Client();
    ~Client();
    void connectToServer(const std::string& serverIP, int serverPort);
    void sendMessage(const std::string& msg);
    void receiveMessage();
};

#endif
