#pragma once


#include "command.h"
#include "timer.h"
#include "unconfirm.h"

class User{
    public:
    char username[8];
    Command *buffer;
    int sock;
    Timer timer;
    Unconfirm unconfirm;

    User();
    bool operator==(char* username);
    void clearQueueUnconfirm();
    void disconnect();
    void updateUserInfo(char* username);
};


