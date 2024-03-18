#include "protocol.h"

// Timer work
void Protocol::addTimer(){
    int td = timerfd_create(CLOCK_REALTIME, 0);
    struct itimerspec timer;
    clock_gettime(CLOCK_REALTIME, &timer.it_value);
    timer.it_interval.tv_sec = 3;
    timer.it_interval.tv_nsec = 0;
    timerfd_settime(td, TFD_TIMER_ABSTIME, &timer, NULL);
    timerQueue.push(td);
}

void Protocol::removeTimer(){
    int td = timerQueue.front();
    timerQueue.pop();
    shutdown(td, SHUT_RDWR);
    close(td);
}

void Protocol::clearTimerQueue(){
    for(int i = 0; i < timerQueue.size(); i++){
        removeTimer();
    }
}

// unconfirm work
void Protocol::addToUnconfirm(Command& buffer){
    unconfirm.push(buffer);
}

void Protocol::removeFromUnconfirm(){
    unconfirm.pop();
}

void Protocol::clearUnconfirm(char* name){
    char filePuth[24];
    sprintf(filePuth, "../offline/%s.txt", name);
    int file = open(filePuth, O_WRONLY);
    for(int i = 0; i < unconfirm.size(); i++){
        Command filer(unconfirm.front());
        unconfirm.pop();
        filer.header.type = 2;
        write(file, &filer, filer.header.len);
    }
}