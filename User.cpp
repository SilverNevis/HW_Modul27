#include "User.h"

User::User(const std::string& login, const std::string& password, const std::string& name)
    : login(login), password(password), name(name) {}

std::string User::getLogin() const {
    return login;
}

std::string User::getPassword() const {
    return password;
}

std::string User::getName() const {
    return name;
}
