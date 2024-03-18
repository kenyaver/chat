#include "timer.h"

Timer::Timer(int timerfd){
    this->timerfd = timerfd;
}

Timer Timer::operator=(Timer& a){
    this->timerfd = a.timerfd;
    return *this;
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
    Timer td = timerQueue.front();
    shutdown(td.timerfd, SHUT_RDWR);
    close(td.timerfd);
    timerQueue.pop();
}

void clearTimerQueue(){
    for(int i = 0; i < timerQueue.size(); i++){
        Timer tmp = timerQueue.front();
        tmp.removeTimer();
    }
}