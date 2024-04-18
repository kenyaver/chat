#include "session.h"
#include <sys/poll.h>
#include <iostream>


Session::Session(int sock){
    std::cout << "start init user" << std::endl;
    User you;
    you.sock = sock;
    this->protocol.addUser(you);
}

void Session::worker(){
    std::cout << "start session user" << std::endl;
    int err = this->protocol.helloUser();
    if(err == -1){
        this->protocol.user->disconnect();
        return;
    }
    std::cout << "success hello user" << std::endl;
    struct pollfd fds[2];
    nfds_t nfd = 2;
    fds[0].fd = this->protocol.user->sock;
    fds[0].events = POLLIN | POLLRDHUP | POLLHUP;
    fds[1].fd = this->protocol.user->timer.getFirstTimer();
    fds[1].events = POLLIN;
    int ret = 0;
    while(ret != -1){
        ret = poll(fds, 2, -1);
        if(ret > 0){
            if(fds[0].revents != 0){
                if(fds[0].revents != POLLRDHUP && fds[0].revents != POLLHUP){
                    this->protocol.handleCommand();
                    fds[0].revents = 0;
                } else {
                    fds[0].revents = 0;
                    break;
                }
            } else {
                this->protocol.handleTimer();
                fds[1].revents = 0;
            }
        }
    }
    this->protocol.clearUser();
}
