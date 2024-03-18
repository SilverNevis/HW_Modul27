#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "User.h"
#include "Trie.h"
#include <Winsock2.h>
#include "MySQLConnector.h"

class Chat {
private:
	std::vector<User> users;
	std::unordered_set<std::string> user_logins;
	std::unordered_map<std::string, std::string> messages;
	Trie trie;
	SOCKET serverSocket;
	SOCKET clientSocket;
	MySQLConnector* connector;
	MYSQL* connection;

public:
	void registerUser(const std::string& login, const std::string& password, const std::string& name);
	bool loginUser(const std::string& login, const std::string& password);
	void sendMessage(const std::string& from, const std::string& to, const std::string& message);
	std::vector<std::string> getMessages(const std::string& user);
	std::string readMessages(const std::string& user);
	std::vector<std::string> autocomplete(const std::string& prefix);
	void startNetworkService(int port);
	void handleClient(int clientSocket);
	void saveMessageToDatabase(int senderId, int receiverId, const std::string& message); 
	std::vector<std::string> loadMessagesFromDatabase(int userId); 
	Chat(const std::string& host, const std::string& user, const std::string& password, const std::string& database);
	~Chat();
};


