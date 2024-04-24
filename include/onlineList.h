#pragma once

#include <list>
#include "user.h"

class OnlineList{
    std::list<User> onlineList;
    public:
    void addUser(User& newUser);

    // \returns возвращает указатель на пользователя в случае успеха, либо NULL, если не удалось найти пользователя
    User* findUser(char* username);

    // \param username юзернейм, по которому определяется, какого пользователя исключить
    void removeUser(char* username);
};

inline OnlineList onlineList;
