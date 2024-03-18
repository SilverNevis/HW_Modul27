#pragma once
#ifndef MYSQLCONNECTOR_H
#define MYSQLCONNECTOR_H

#include <mysql.h>
#include <iostream>
#include <string>

class MySQLConnector {
private:
    MYSQL* connection;

public:
    MySQLConnector(const std::string& host, const std::string& user, const std::string& password, const std::string& database, unsigned int port = 3306);
    ~MySQLConnector();
    MYSQL* getConnection() const;
    int executeQuery(MYSQL* connection, const std::string& query);
};

#endif



