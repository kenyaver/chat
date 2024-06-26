#include "user.h"
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

User::User(){
    this->buffer = (Command*)calloc(1, sizeof(Header));
}

bool User::operator==(char* username){
    return !strcmp(this->username, username);
}

void User::clearQueueUnconfirm(){
    for(auto i = 0; i < this->unconfirm.size(); i++){
        this->unconfirm.pop();
    }
}

void User::disconnect(){
    shutdown(this->sock, SHUT_RDWR);
    close(this->sock);
    this->sock = -1;
    this->clearQueueUnconfirm();
    this->timer.clearTimerQueue();
}
