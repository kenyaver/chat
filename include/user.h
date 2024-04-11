#pragma once


#include "command.h"
#include "timer.h"

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
    void updateUserInfo(char* username);
};


