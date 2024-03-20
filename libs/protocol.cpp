#include "protocol.h"

#include "check.h"
#include "function.h"

void Protocol::addUser(User& user){
    onlineList.addUser(user);
    this->user = onlineList.findUser(user.username);
}

void Protocol::processRecvCommand(){
    recvCommand(this->user->sock, this->user->bufferRecv);
    if(this->user->bufferRecv->header.type == 1){
        this->partner = this->onlineList.findUser(this->user->bufferRecv->header.DST);
        if(partner != NULL){
            this->partner->bufferSend = (Command*)realloc(this->partner->bufferSend, this->user->bufferRecv->header.len);
            memcpy(this->partner->bufferSend, this->user->bufferRecv, this->user->bufferRecv->header.len);
            this->processSendCommand();
        } else {
            this->offline.setPath(this->user->bufferRecv->header.DST);
            this->offline.writeFile(*this->user->bufferRecv);
        }
    } else {
        this->user->timer.removeTimer();
        this->user->unconfirm.removeFromUnconfirm();
        //TODO: вынести в отдельную функцию (повторение кода - ай-ай-ай)
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
}

void Protocol::processSendCommand(){
    sendCommand(this->partner->sock, *this->partner->bufferSend);
    this->partner->timer.addTimer();
    this->partner->unconfirm.addToUnconfirm(*this->partner->bufferSend);
}

void Protocol::clearUser(){
    this->user->timer.clearTimerQueue();
    this->onlineList.removeUser(this->user->username);
    this->offline.setPath(this->user->username);
    for(int i = 0; i < this->user->unconfirm.size(); i++){
        
        Command *tmp = &this->user->unconfirm.getCommand();
        tmp->header.type = 2;
        this->offline.writeFile(*tmp);
        
        //TODO: вынести в отдельную функцию изменение команды до ответного вида
        tmp->header.type = 1;
        tmp->header.len = sizeof(Header) + 4;
        tmp = (Command*)realloc(tmp, tmp->header.len);
        memcpy(tmp->message, "300", 4);
        User* recver = this->onlineList.findUser(tmp->header.SRC); 
        if(recver != nullptr){
            sendCommand(recver->sock, *tmp);
        }
    
        this->user->unconfirm.removeFromUnconfirm();
    }
}