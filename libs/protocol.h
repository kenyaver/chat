#pragma once

#include "check.h"
#include "function.h"
#include "command.h"
#include "user.h"
#include "timer.h"


class Protocol{
    public:
    std::queue<Command> unconfirm;

    // unconfirm
    void addToUnconfirm(Command& buffer);
    void removeFromUnconfirm();
    void clearUnconfirm(char* name);

    // recv/send commands
    void processRecvCommand();
    void processSendCommand();
    
};