#pragma once

#include "aheader.h"

class Timer{
    std::queue<int> timerQueue;

    public:
    void addTimer();
    void removeTimer();
    void clearTimerQueue();
};