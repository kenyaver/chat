#include "protocol.h"

#include "check.h"
#include "function.h"

void Protocol::addUser(User& user){
    onlineList.addUser(user);
    this->user = onlineList.findUser(user.username);
}

void Protocol::handleCommand(){
    recvCommand(this->user->sock, this->user->bufferRecv);
    this->processSendCommand();
    if(this->user->bufferRecv->header.type == 1){
        this->partner->unconfirm.push(*this->partner->bufferSend);
        this->partner->timer.addTimer();
    }
}

void Protocol::processSendCommand(){
    this->partner = this->onlineList.findUser(this->user->bufferRecv->header.DST);
    if(partner != NULL){
        this->partner->bufferSend = (Command*)realloc(this->partner->bufferSend, this->user->bufferRecv->header.len);
        memcpy(this->partner->bufferSend, this->user->bufferRecv, this->user->bufferRecv->header.len);
        sendCommand(this->partner->sock, *this->partner->bufferSend);
    } else {
        this->offline.setPath(this->user->bufferRecv->header.DST);
        this->offline.writeFile(*this->user->bufferRecv);
    }
}

void Protocol::clearUser(){
    this->user->timer.clearTimerQueue();
    this->onlineList.removeUser(this->user->username);
    this->offline.setPath(this->user->username);
    for(int i = 0; i < this->user->unconfirm.size(); i++){
        
        Command *tmp = &this->user->unconfirm.front();
        tmp->header.type = 2;
        this->offline.writeFile(*tmp);
        
        tmp->header.type = 1;
        tmp->header.len = sizeof(Header) + 4;
        tmp = (Command*)realloc(tmp, tmp->header.len);
        memcpy(tmp->message, "300", 4);
        User* recver = this->onlineList.findUser(tmp->header.SRC); 
        if(recver != nullptr){
            sendCommand(recver->sock, *tmp);
        }
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