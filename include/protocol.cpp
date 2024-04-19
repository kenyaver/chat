#include "protocol.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <iostream>
#include <algorithm>

void Protocol::addUser(User& user){
    onlineList.addUser(user);
    this->user = onlineList.findUser(user.username);
}

void Protocol::handleCommand(){
    int byte = recvCommand(this->user->sock, this->user->bufferRecv);
    if(byte == -1){
        std::cout << "bad handle command" << std::endl;
        return;
    }
    this->processSendCommand();
}

int Protocol::helloUser(){
    int byte;
    byte = recvCommand(this->user->sock, this->user->bufferRecv);
    if(byte == -1){
        std::cout << "bad handle hello user" << std::endl;
        return -1;
    }
    memcpy(this->user->username, this->user->bufferRecv->header.SRC, 8);
    this->processSendCommand();
    return 0;
}

void Protocol::processSendCommand(){
    this->partner = onlineList.findUser(this->user->bufferRecv->header.DST);
    if(partner != NULL){
        std::cout << "partner online" << std::endl;
        this->partner->bufferSend = (Command*)realloc(this->partner->bufferSend, this->user->bufferRecv->header.len);
        // memcpy(this->partner->bufferSend, this->user->bufferRecv, this->user->bufferRecv->header.len);
        this->partner->bufferSend = std::move(this->user->bufferRecv);
        int err = sendCommand(this->partner->sock, *this->partner->bufferSend);
        if(err == -1){
            std::cout << "bad send to user" << std::endl;
            return;
        }
        if(this->user->bufferSend->header.type == 0){
            this->partner->unconfirm.push(*this->partner->bufferSend);
            this->partner->timer.addTimer();
        }
    } else {
        std::cout << "partner offline" << std::endl;
        this->offline.setPath(this->user->bufferRecv->header.DST);
        this->user->bufferRecv->header.type = 2;
        this->offline.writeFile(*this->user->bufferRecv);
        std::cout << "command writed in file" << std::endl;
        this->user->bufferRecv->header.type = 1;
        this->user->bufferRecv->header.len = sizeof(Header) + 4;
        this->user->bufferRecv = (Command*)realloc(this->user->bufferRecv, this->user->bufferRecv->header.len);
        std::swap(this->user->bufferRecv->header.SRC, this->user->bufferRecv->header.DST);
        memcpy(this->user->bufferRecv->message, "300", 4);
        int err = sendCommand(this->user->sock, *this->user->bufferRecv);
        if(err == -1){
            std::cout << "bad send answer" << std::endl;
        }
    }
}

void Protocol::clearUser(){
    this->user->timer.clearTimerQueue();
    onlineList.removeUser(this->user->username);
    this->offline.setPath(this->user->username);
    for(int i = 0; i < this->user->unconfirm.size(); i++){
        Command *tmp = &this->user->unconfirm.front();
        tmp->header.type = 2;
        this->offline.writeFile(*tmp);
        
        tmp->header.type = 1;
        tmp->header.len = sizeof(Header) + 4;
        tmp = (Command*)realloc(tmp, tmp->header.len);
        memcpy(tmp->message, "300", 4);
        User* recver = onlineList.findUser(tmp->header.SRC); 
        if(recver != NULL){
            sendCommand(recver->sock, *tmp);
        }
        this->user->unconfirm.pop();
    }
}

void Protocol::clearUserList(){
    onlineList.clearUserDB();
}

void Protocol::handleTimer(){
    uint64_t exp;
    int byte = read(this->user->timer.getFirstTimer(), &exp, sizeof(exp));
    if(byte != -1){
        this->clearUser();
    }
}
