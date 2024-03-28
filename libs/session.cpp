#include "session.h"

Session::Session(int sock){
    User you;
    you.sock = sock;
    this->protocol.addUser(you);
}

int Session::waitFDs(){
    struct pollfd fds[2];
    nfds_t nfd = 2;
    fds[0].fd = this->protocol.user->sock; // сокет-дескриптор пользователя
    fds[0].events = POLLIN | POLLRDHUP;

    fds[1].fd = this->protocol.user->timer.getFirstTimer(); // дескриптор таймера
    fds[1].events = POLLIN | POLLRDHUP;
    int ret = 0;
    while(ret != -1){
        ret = poll(fds, 2, -1);
        if(ret > 0){
            if(fds[0].revents != 0){
                fds[0].revents = 0;
                // TODO: сделать проверку, что ивентом является не HUP
                this->handleCommand();
            } else {
                fds[1].revents = 0;
                this->handleTimer();
            }
        }
    }
    this->end();
}

void Session::handleCommand(){
    recvCommand(this->protocol.user->sock, this->protocol.user->bufferRecv);

    if(this->protocol.user->bufferRecv->header.type == 0){
        // отправка коамнды получателю по юзернейму
    } else {
        // отправка команды получателю по юзернейму
        // удаление первого в очереди таймера
        // удаление первого в очереди анконфирма
    }
}

void Session::handleTimer(){
    uint64_t exp;
    int byte = read(this->protocol.user->timer.getFirstTimer(), &exp, sizeof(exp));
    if(byte != -1){
        this->end();
    }
}

void Session::end(){
    this->protocol.clearUserList();
}