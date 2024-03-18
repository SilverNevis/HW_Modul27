#pragma once
#include <string>

class User {
private:
    std::string login;
    std::string password;
    std::string name;

public:
    User(const std::string& login, const std::string& password, const std::string& name);

    std::string getLogin() const;
    std::string getPassword() const;
    std::string getName() const;
};

