#pragma once

#include "check.h"
#include "function.h"

class User{
    public:
    User();
    User(char* username, int status);
    char username[8];
    int sockfd;
    int status;
    char bufferRecv[1032]; // буффер для принятия сообщений
    char bufferSend[1032]; // буффер для отправки сообщений

    bool operator==(User& a) noexcept;

    void clearUser(char* searchReader);

    ~User();
};

inline std::vector<User> userDB;