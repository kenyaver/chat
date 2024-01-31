#pragma once

#include "check.h"
#include "function.h"

class User{
    public:
    char username[8];
    int sockfd;
    int status;
    char bufferRecv[1032]; // буффер для принятия сообщений
    char bufferSend[1032]; // буффер для отправки сообщений
    
    User();
    User(char* username, int sockfd, int status);
    User(char* username, int status);
    User(int sockfd);
    // User(User& a);
    bool operator==(User& a) noexcept;

    void disconnectUser();
    void closeSocket();
    ~User();
};

inline std::vector<User> userDB;