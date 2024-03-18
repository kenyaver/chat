#pragma once

#include "check.h"
#include "function.h"
#include "command.h"


class Protocol{
    public:
    std::vector<Command> unconfirm;
    std::queue<int> timerQueue;

    void addTimer();
    void removeTimer();
    void clearTimerQueue();


    void addToUnconfirm(Command& buffer);
    void removeFromUnconfirm(int messageID);
    void saveUnconfirm(Command& offlineCommand);
};