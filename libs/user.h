#pragma once


#include "command.h"
#include "timer.h"
#include "offline.h"

class User{
    public:
    char username[8];
    Command *bufferSend;
    Command *bufferRecv;
    int sock;
    Timer timer;
    std::queue<Command> unconfirm;


    bool operator==(char* username);
    void clearQueueUnconfirm();
    void disconnect();
    // используетя после принятия сервером первой команды от клиента для заполнения полей user
    void updateUserInfo(char* username);
};


