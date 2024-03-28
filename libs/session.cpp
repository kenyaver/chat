#include "session.h"

Session::Session(int sock){
    User you;
    you.sock = sock;
    this->protocol.addUser(you);
}

int Session::waitFDs(){
    struct pollfd fds[2];
    nfds_t nfd = 2;
    fds[0].fd = this->protocol.user->sock;
    fds[0].events = POLLIN | POLLRDHUP;

    fds[1].fd = this->protocol.user->timer.getFirstTimer();
    fds[1].events = POLLIN | POLLRDHUP;
    int ret = 0;
    while(ret != -1){
        ret = poll(fds, 2, -1);
        if(ret > 0){
            if(fds[0].revents != 0){
                if(fds[0].revents != POLLRDHUP){
                    this->protocol.processRecvCommand();
                } else {
                    break;
                }
            } else {
                this->handleTimer();
            }
            fds[0].revents = 0;
        }
    }
    this->end();
}

void Session::handleTimer(){
    uint64_t exp;
    int byte = read(this->protocol.user->timer.getFirstTimer(), &exp, sizeof(exp));
    if(byte != -1){
        this->end();
    }
}

void Session::end(){
    this->protocol.clearUser();
}