#include "aheader.h"

class Timer{
    int timerfd;

    public:
    Timer(int timerfd);
    Timer operator=(Timer& a);

    void addTimer();
    void removeTimer();
};

inline std::queue<Timer> timerQueue;

void clearTimerQueue();