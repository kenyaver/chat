#pragma once

#include <queue>

class Timer{
    std::queue<int> timerQueue;

    public:
    int getFirstTimer();
    void addTimer();
    void removeTimer();
    void clearTimerQueue();
};