#pragma once

#include "check.h"
#include "function.h"
#include "command.h"

class User{
    public:
    int sock;
    char username[8];
    Command buffrecv;
    Command buffsend;
    void disconnect();
};

class UserDB{
    std::list<User> userDB;
    public:
    // используется при подключении клиента
    void addUser(User& newUser);
    // используетя после принятия сервером первой команды от клиента для заполнения полей user
    void updateUserInfo(User& user);
    // используется для поиска искомого партнера для последующей передачи тому команды
    User findUser(char* username);
    // используется при обнаружении факта отключения пользователя от сервера
    void removeUser();
    // используется при завершении работы сервера
    void clearUserDB();
};