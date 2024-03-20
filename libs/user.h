#pragma once


#include "command.h"
#include "timer.h"
#include "unconfirm.h"
#include "offline.h"

class User{
    public:
    char username[8];
    Command *bufferSend;
    Command *bufferRecv;
    int sock;
    
    Timer timer;
    Unconfirm unconfirm;
    Offline offline;
    bool operator==(char* username);
    void disconnect();
    // используетя после принятия сервером первой команды от клиента для заполнения полей user
    void updateUserInfo(char* username);
};

class OnlineList{
    std::list<User> onlineList;
    public:
    // используется при подключении клиента
    void addUser(User& newUser);

    // используется для поиска искомого партнера для последующей передачи тому команды
    // \param username юзернейм, по которому ищется нужный пользователь
    // \returns возвращает указатель на пользователя в случае успеха, либо NULL, если не удалось найти пользователя
    User* findUser(char* username);

    // используется при обнаружении факта отключения пользователя от сервера
    // \param username юзернейм, по которому определяется, какого пользователя исключить
    void removeUser(char* username);
    // используется при завершении работы сервера
    void clearUserDB();
};
