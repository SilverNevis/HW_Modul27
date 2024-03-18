#include "Client.h"
#include <iostream>
#include <ws2tcpip.h>

Client::Client() {
    // Инициализация Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << "\n";
        return;
    }

    // Создание сокета для подключения к серверу
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << "\n";
        WSACleanup();
        return;
    }
}

Client::~Client() {
    // Закрытие сокета и очистка Winsock
    closesocket(clientSocket);
    WSACleanup();
}

void Client::connectToServer(const std::string& serverIP, int serverPort) {
    // Настройка сокета
    sockaddr_in hints;
    hints.sin_family = AF_INET;
    hints.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIP.c_str(), &(hints.sin_addr));

    // Подключение к серверу
    int connResult = connect(clientSocket, (struct sockaddr*)&hints, sizeof(hints));
    if (connResult == SOCKET_ERROR) {
        std::cout << "connect failed with error: " << WSAGetLastError() << "\n";
        closesocket(clientSocket);
        WSACleanup();
        return;
    }
}

void Client::sendMessage(const std::string& msg) {
    // Отправка сообщения серверу
    int sendResult = send(clientSocket, msg.c_str(), msg.size() + 1, 0);
    if (sendResult == SOCKET_ERROR) {
        std::cout << "send failed with error: " << WSAGetLastError() << "\n";
    }
}

void Client::receiveMessage() {
    char buf[4096];
    ZeroMemory(buf, 4096);

    // Получение сообщения от сервера
    int bytesReceived = recv(clientSocket, buf, 4096, 0);
    if (bytesReceived > 0) {
        // Вывод полученного сообщения
        std::cout << "SERVER> " << std::string(buf, 0, bytesReceived) << "\n";
    }
}
