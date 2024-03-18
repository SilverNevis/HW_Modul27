#include "MySQLConnector.h"

MySQLConnector::MySQLConnector(const std::string& host, const std::string& user, const std::string& password, const std::string& database, unsigned int port) {
    connection = mysql_init(nullptr);

    if (connection == nullptr) {
        std::cerr << "Error: mysql_init() failed" << std::endl;
        return;
    }

    if (!mysql_real_connect(connection, host.c_str(), user.c_str(), password.c_str(), database.c_str(), port, nullptr, 0)) {
        std::cerr << "Error: mysql_real_connect() failed: " << mysql_error(connection) << std::endl;
        mysql_close(connection);
        connection = nullptr;
        return;
    }
}

MySQLConnector::~MySQLConnector() {
    if (connection != nullptr) {
        mysql_close(connection);
    }
}

MYSQL* MySQLConnector::getConnection() const {
    return connection;
}

int MySQLConnector::executeQuery(MYSQL* connection, const std::string& query) {
    if (mysql_query(connection, query.c_str()) != 0) {
        std::cerr << "Error: Failed to execute query: " << mysql_error(connection) << std::endl;
        return -1;
    }
    return 0;
}


