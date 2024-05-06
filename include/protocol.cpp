#include "protocol.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <iostream>
#include <fstream>
#include <algorithm>

void Protocol::addUser(User& user){
    onlineList.addUser(user);
    this->user = onlineList.findUser(user.username);
}

void Protocol::handleCommand(){
    int byte = recvCommand(this->user->sock, this->user->buffer);
    if(byte == -1){
        std::cout << "bad handle command" << std::endl;
        return;
    }
    this->processSendCommand();
}

int Protocol::helloUser(){
    int byte;
    byte = recvCommand(this->user->sock, this->user->buffer);
    if(byte == -1){
        std::cout << "bad handle hello user" << std::endl;
        return -1;
    }
    memcpy(this->user->username, this->user->buffer->header.SRC, 8);
    this->processSendCommand();
    this->user->offline.setPath(this->user->username);
    this->user->offline.readFile(this->user->buffer);
    return 0;
}

void Protocol::processSendCommand(){
    this->partner = onlineList.findUser(this->user->buffer->header.DST);
    if(this->partner != NULL){
        std::cout << "partner online" << std::endl;
        int err = sendCommand(this->partner->sock, *this->user->buffer);
        if(err == -1){
            std::cout << "bad send to user" << std::endl;
            return;
        }
        if(this->user->buffer->header.type == 0){
            this->partner->unconfirm.push(*this->user->buffer);
            this->partner->timer.addTimer();
        }
    } else {
        std::cout << "partner offline" << std::endl;
        this->user->offline.setPath(this->user->buffer->header.DST);
        this->user->buffer->header.type = 2;
        this->user->offline.writeFile(*this->user->buffer);
        this->user->buffer->header.type = 1;
        this->user->buffer->header.len = sizeof(Header) + 3;
        this->user->buffer = (Command*)realloc(this->user->buffer, this->user->buffer->header.len);
        std::swap(this->user->buffer->header.SRC, this->user->buffer->header.DST);
        strcpy(this->user->buffer->message, "300");
        int err = sendCommand(this->user->sock, *this->user->buffer);
        if(err == -1){
            std::cout << "bad send answer" << std::endl;
        }
    }
}

void Protocol::clearUser(){
    this->user->timer.clearTimerQueue();
    onlineList.removeUser(this->user->username);
    this->user->offline.setPath(this->user->username);
    while(this->user->unconfirm.size() != 0){
        Command *tmp = this->user->unconfirm.front();
        tmp->header.type = 2;
        this->user->offline.writeFile(*tmp);
        User* recver = onlineList.findUser(tmp->header.SRC);
        if(recver != NULL){
            tmp->header.type = 1;
            tmp->header.len = sizeof(Header) + 3;
            tmp = (Command*)realloc(tmp, tmp->header.len);
            strcpy(tmp->message, "300");
            sendCommand(recver->sock, *tmp);
        }
        this->user->unconfirm.pop();
    }
}

void Protocol::handleTimer(){
    uint64_t exp;
    int byte = read(this->user->timer.getFirstTimer(), &exp, sizeof(exp));
    if(byte != -1){
        this->clearUser();
    }
}
