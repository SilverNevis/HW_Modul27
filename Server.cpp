#include "Server.h"
#include <iostream>

Server::Server() {
    // Инициализация Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << "\n";
        return;
    }

    // Создание сокета для прослушивания входящих соединений
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << "\n";
        WSACleanup();
        return;
    }

    // Настройка сокета
    sockaddr_in hints;
    hints.sin_family = AF_INET;
    hints.sin_port = htons(7777); 
    hints.sin_addr.S_un.S_addr = INADDR_ANY;

    // Привязка сокета
    iResult = bind(serverSocket, (struct sockaddr*)&hints, sizeof(hints));
    if (iResult == SOCKET_ERROR) {
        std::cout << "bind failed with error: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    // Прослушивание входящих соединений
    iResult = listen(serverSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cout << "listen failed with error: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    // Очистка набора сокетов
    FD_ZERO(&master);
}

Server::~Server() {
    // Закрытие сокета и очистка Winsock
    closesocket(serverSocket);
    WSACleanup();
}

void Server::run() {
    while (true) {
        fd_set copy = master;

        int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

        for (int i = 0; i < socketCount; i++) {
            SOCKET sock = copy.fd_array[i];
            if (sock == serverSocket) {
                // Принимаем новое соединение
                SOCKET client = accept(serverSocket, nullptr, nullptr);

                // Добавляем нового клиента в набор сокетов
                FD_SET(client, &master);

                // Отправляем приветственное сообщение новому клиенту
                std::string welcomeMsg = "Welcome to the chat server!\r\n";
                send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
            }
            else {
                char buf[4096];
                ZeroMemory(buf, 4096);

                // Принимаем сообщение
                int bytesIn = recv(sock, buf, 4096, 0);
                if (bytesIn <= 0) {
                    // Закрываем соединение
                    closesocket(sock);
                    FD_CLR(sock, &master);
                }
                else {
                    // Отправляем сообщение всем клиентам, кроме отправителя
                    for (int i = 0; i < master.fd_count; i++) {
                        SOCKET outSock = master.fd_array[i];
                        if (outSock != serverSocket && outSock != sock) {
                            send(outSock, buf, bytesIn, 0);
                        }
                    }
                }
            }
        }
    }
}

void Server::sendToClient(int clientSocket, const std::string& msg) {
    send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}

void Server::broadcast(const std::string& msg) {
    for (int i = 0; i < master.fd_count; i++) {
        SOCKET outSock = master.fd_array[i];
        if (outSock != serverSocket) {
            send(outSock, msg.c_str(), msg.size() + 1, 0);
        }
    }
}

void Server::removeClient(int clientSocket) {
    closesocket(clientSocket);
    FD_CLR(clientSocket, &master);
}
