#pragma once

#include "timer.h"
#include "unconfirm.h"
#include "user.h"


class Protocol{
    Online online;
    Timer timer;
    Unconfirm unconfirm;  
    public:
    // recv/send commands
    void processRecvCommand();
    void processSendCommand();
    
    //disconnect
    void disconnectClient();
};
