#include "Server.h"
#include "Client.h"
#include <iostream>
#include "Chat.h" 
#include "MySQLConnector.h" 

int main() {

    // Выбор режима работы
    char choice;
    std::cout << "Run as server or client? (s/c): ";
    std::cin >> choice;

    if (choice == 's') {
        Server server;
        server.run();
    }
    else if (choice == 'c') {
        Client client;
        client.connectToServer("127.0.0.1", 7777); // 

        std::string msg;
        do {
            std::cout << "> ";
            std::getline(std::cin, msg);

            client.sendMessage(msg);

            if (msg != "exit") {
                client.receiveMessage();
            }
        } while (msg != "exit");
    }
    else if (choice == 'r') { // Добавлен новый выбор для чата
        setlocale(LC_ALL, "RUSSIAN");

        // Передаем параметры конструктору Chat
        Chat chat("localhost", "root", "2982680", "chat2");

        std::string current_user;

        while (true) {
            std::cout << "Выберите действие:\n1. Регистрация\n2. Вход\n3. Выход\n4. Отправить сообщение\n5. Прочитать сообщения\n6. Автодополнение\n7. Выход из программы\n";
            int choice;
            std::cin >> choice;

            try {
                if (choice == 1) {
                    std::string login, password, name;
                    std::cout << "Введите логин: ";
                    std::cin >> login;
                    std::cout << "Введите пароль: ";
                    std::cin >> password;
                    std::cout << "Введите имя: ";
                    std::cin >> name;
                    chat.registerUser(login, password, name);
                }
                else if (choice == 2) {
                    if (current_user.empty()) {
                        std::string login, password;
                        std::cout << "Введите логин: ";
                        std::cin >> login;
                        std::cout << "Введите пароль: ";
                        std::cin >> password;
                        if (chat.loginUser(login, password)) {
                            current_user = login;
                            std::cout << "Вы вошли как: " << current_user << "\n";
                        }
                        else {
                            std::cout << "Ошибка входа. Проверьте логин и пароль.\n";
                        }
                    }
                    else {
                        std::cout << "Вы уже вошли как: " << current_user << "\n";
                    }
                }
                else if (choice == 3) {
                    current_user = "";
                    std::cout << "Выход выполнен.\n";
                }
                else if (choice == 4) {
                    if (current_user.empty()) {
                        std::cout << "Вы не вошли в чат. Войдите или зарегистрируйтесь.\n";
                    }
                    else {
                        std::string to, message;
                        std::cout << "Введите получателя: ";
                        std::cin >> to;
                        std::cin.ignore();
                        std::cout << "Введите сообщение: ";
                        std::getline(std::cin, message);
                        chat.sendMessage(current_user, to, message);
                        std::cout << "Сообщение отправлено.\n";
                    }
                }
                else if (choice == 5) {
                    if (current_user.empty()) {
                        std::cout << "Вы не вошли в чат. Войдите или зарегистрируйтесь.\n";
                    }
                    else {
                        std::cout << chat.readMessages(current_user) << std::endl;
                    }
                }
                else if (choice == 6) {
                    if (current_user.empty()) {
                        std::cout << "Вы не вошли в чат. Войдите или зарегистрируйтесь.\n";
                    }
                    else {
                        std::string prefix;
                        std::cout << "Введите префикс: ";
                        std::cin >> prefix;
                        std::vector<std::string> words = chat.autocomplete(prefix);
                        for (const auto& word : words) {
                            std::cout << word << std::endl;
                        }
                    }
                }
                else if (choice == 7) {
                    break;
                }
                else {
                    std::cout << "Неверный выбор. Попробуйте еще раз.\n";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Ошибка: " << e.what() << std::endl;
            }
        }
    }
    else {
        std::cout << "Invalid choice.\n";
    }

    return 0;
}
