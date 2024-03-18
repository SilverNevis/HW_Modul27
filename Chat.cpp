#include "Chat.h"
#include <iostream>
#include <stdexcept>
#include <ws2tcpip.h>
#include "MySQLConnector.h"
#include <sys/types.h>
#include <Winsock2.h>

// ����������� ������ Chat
Chat::Chat(const std::string& host, const std::string& user, const std::string& password, const std::string& database) {
    // ������������� ����������� � ���� ������
    connector = new MySQLConnector(host, user, password, database);
    connection = connector->getConnection();
    if (connection == nullptr) {
        throw std::runtime_error("Failed to connect to the database");
    }
}

Chat::~Chat() {
    // �������� ���������� � ����� ������
    if (connection != nullptr) {
        mysql_close(connection);
    }
    delete connector;
}

void Chat::saveMessageToDatabase(int senderId, int receiverId, const std::string& message) {
    // ���������� ������� ��� ������� ��������� � ���� ������
    std::string query = "INSERT INTO Messages (sender_id, receiver_id, message) VALUES (" + std::to_string(senderId) + ", " + std::to_string(receiverId) + ", '" + message + "')";

    // ���������� �������
    if (mysql_query(connection, query.c_str()) != 0) {
        throw std::runtime_error("Failed to execute query: " + std::string(mysql_error(connection)));
    }
}

std::vector<std::string> Chat::loadMessagesFromDatabase(int userId) {
    std::vector<std::string> messages;

    // ���������� ������� ��� �������� ��������� �� ���� ������ ��� ����������� ������������
    std::string query = "SELECT message FROM Messages WHERE receiver_id = " + std::to_string(userId);

    // ���������� �������
    if (mysql_query(connection, query.c_str()) != 0) {
        throw std::runtime_error("Failed to execute query: " + std::string(mysql_error(connection)));
    }

    // ��������� ����������� �������
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) {
        throw std::runtime_error("Failed to store result set: " + std::string(mysql_error(connection)));
    }

    // ��������� ����������� �������
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr) {
        messages.push_back(row[0]);
    }

    // ������������ ��������
    mysql_free_result(result);

    return messages;
}


void Chat::registerUser(const std::string& login, const std::string& password, const std::string& name) {
    // ����������� � ���� ������
    MySQLConnector connector("localhost", "root", "2982680", "chat2");
    MYSQL* connection = connector.getConnection();
    if (connection == nullptr) {
        throw std::runtime_error("Failed to connect to the database");
    }

    // �������� ������� ������������ � ����� �������
    std::string query = "SELECT * FROM Users WHERE login='" + login + "'";
    if (connector.executeQuery(connection, query) != 0) {
        throw std::runtime_error("Failed to execute query");
    }
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) {
        throw std::runtime_error("Failed to get result set");
    }
    int num_rows = mysql_num_rows(result);
    mysql_free_result(result);
    if (num_rows > 0) {
        throw std::runtime_error("User with this login already exists");
    }

    // ����������� ������ ������������
    query = "INSERT INTO Users (login, password, name) VALUES ('" + login + "', '" + password + "', '" + name + "')";
    if (connector.executeQuery(connection, query) != 0) {
        throw std::runtime_error("Failed to register user");
    }

    // �������� ����������
    mysql_close(connection);
}

bool Chat::loginUser(const std::string& login, const std::string& password) {
    // ����������� � ���� ������
    MySQLConnector connector("localhost", "root", "2982680", "chat2");
    MYSQL* connection = connector.getConnection();
    if (connection == nullptr) {
        throw std::runtime_error("Failed to connect to the database");
    }

    // ����� ������������ � �������� ������� � �������
    std::string query = "SELECT * FROM Users WHERE login='" + login + "' AND password='" + password + "'";
    if (connector.executeQuery(connection, query) != 0) {
        throw std::runtime_error("Failed to execute query");
    }
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) {
        throw std::runtime_error("Failed to get result set");
    }
    int num_rows = mysql_num_rows(result);
    mysql_free_result(result);

    // �������� ����������� �������
    if (num_rows == 1) {
        std::cout << "Login successful\n";
    }
    else {
        std::cerr << "Invalid username or password\n";
    }

    // �������� ����������
    mysql_close(connection);
}

void Chat::sendMessage(const std::string& sender, const std::string& receiver, const std::string& message) {
    // ����������� � ���� ������
    MySQLConnector connector("localhost", "root", "2982680", "chat2");
    MYSQL* connection = connector.getConnection();
    if (connection == nullptr) {
        throw std::runtime_error("Failed to connect to the database");
    }

    // ���������� �������
    std::string query = "INSERT INTO Messages (sender_id, receiver_id, message) VALUES ((SELECT id FROM Users WHERE login='" + sender + "'), (SELECT id FROM Users WHERE login='" + receiver + "'), '" + message + "')";

    // ���������� �������
    if (mysql_query(connection, query.c_str()) != 0) {
        throw std::runtime_error("Failed to execute query: " + std::string(mysql_error(connection)));
    }

    // �������� ����������
    mysql_close(connection);
}

std::vector<std::string> Chat::getMessages(const std::string& user) {
    std::vector<std::string> messages;

    // ����������� � ���� ������
    MySQLConnector connector("hostname", "username", "password", "database_name");
    MYSQL* connection = connector.getConnection();
    if (connection == nullptr) {
        throw std::runtime_error("Failed to connect to the database");
    }

    // ���������� �������
    std::string query = "SELECT message FROM Messages WHERE receiver_id = (SELECT id FROM Users WHERE login='" + user + "')";

    // ���������� �������
    if (mysql_query(connection, query.c_str()) != 0) {
        throw std::runtime_error("Failed to execute query: " + std::string(mysql_error(connection)));
    }

    // ��������� ����������� �������
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) {
        throw std::runtime_error("Failed to store result set: " + std::string(mysql_error(connection)));
    }

    // ��������� ����������� �������
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr) {
        messages.push_back(row[0]);
    }

    // ������������ ��������
    mysql_free_result(result);
    mysql_close(connection);

    return messages;
}


std::string Chat::readMessages(const std::string& user) {
    std::vector<std::string> messages = getMessages(user);
    std::string allMessages;
    for (const auto& message : messages) {
        allMessages += message + "\n";
    }
    return allMessages;
}


std::vector<std::string> Chat::autocomplete(const std::string& prefix) {
    std::vector<std::string> completions;

    // ����������� � ���� ������
    MySQLConnector connector("localhost", "root", "2982680", "chat2");
    MYSQL* connection = connector.getConnection();
    if (connection == nullptr) {
        throw std::runtime_error("Failed to connect to the database");
    }

    // ���������� �������
    std::string query = "SELECT login FROM Users WHERE login LIKE '" + prefix + "%'";

    // ���������� �������
    if (mysql_query(connection, query.c_str()) != 0) {
        throw std::runtime_error("Failed to execute query: " + std::string(mysql_error(connection)));
    }

    // ��������� ����������� �������
    MYSQL_RES* result = mysql_store_result(connection);
    if (result == nullptr) {
        throw std::runtime_error("Failed to store result set: " + std::string(mysql_error(connection)));
    }

    // ��������� ����������� �������
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr) {
        completions.push_back(row[0]);
    }

    // ������������ ��������
    mysql_free_result(result);
    mysql_close(connection);

    return completions;
}


void Chat::startNetworkService(int port) {
    // ������������� �������� ����������
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        throw std::runtime_error("Failed to create socket");
    }

    // ��������� ������ �������
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    // �������� ������ � ������ �������
    if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        throw std::runtime_error("Failed to bind socket");
    }

    // ������������� �������� ����������
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        throw std::runtime_error("Failed to listen on socket");
    }

    std::cout << "Server is listening on port " << port << std::endl;

    // �������� �������� ���������� � ��������� ���������
    while (true) {
        // �������� ��������� ����������
        sockaddr_in clientAddress;
        int clientAddressSize = sizeof(clientAddress);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &clientAddressSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }

        // ��������� ��������� �� �������
        handleClient(clientSocket);
    }

    // �������� ������ �������
    closesocket(serverSocket);
}
void Chat::handleClient(int clientSocket) {

    char buffer[1024];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == SOCKET_ERROR) {
        std::cerr << "Failed to receive data from client" << std::endl;
        // ��������� ������ ������
        return;
    }
    else if (bytesRead == 0) {
        // ������ ������ ����������
        std::cout << "Client closed connection" << std::endl;
        return;
    }

    const char* response = "Message received";
    int bytesSent = send(clientSocket, response, strlen(response), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Failed to send response to client" << std::endl;
        // ��������� ������ ��������
        return;
    }

    // �������� ������ �������
    closesocket(clientSocket);
}
