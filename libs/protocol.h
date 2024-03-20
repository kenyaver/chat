#pragma once

#include "timer.h"
#include "unconfirm.h"
#include "user.h"


class protocol{
    OnlineList onlineList;
    Timer timer;
    Unconfirm unconfirm;  
    public:
    void addUser(User& user);
    // recv/send commands
    void processRecvCommand(int sockfd, Command* buffer);
    void processSendCommand(int sockfd, Command& buffer);
    
    //disconnect
    void disconnectClient();
};
