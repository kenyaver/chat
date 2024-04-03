#include "timer.h"
#include <sys/timerfd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

int Timer::getFirstTimer(){
    return timerQueue.front();
}

void Timer::addTimer(){
    int td = timerfd_create(CLOCK_REALTIME, 0);
    struct itimerspec timer;
    clock_gettime(CLOCK_REALTIME, &timer.it_value);
    timer.it_interval.tv_sec = 3;
    timer.it_interval.tv_nsec = 0;
    timerfd_settime(td, TFD_TIMER_ABSTIME, &timer, NULL);
    timerQueue.push(td);
}

void Timer::removeTimer(){
    shutdown(this->timerQueue.front(), SHUT_RDWR);
    close(this->timerQueue.front());
    this->timerQueue.pop();
}

void Timer::clearTimerQueue(){
    for(int i = 0; i < this->timerQueue.size(); i++){
        this->removeTimer();
    }
}