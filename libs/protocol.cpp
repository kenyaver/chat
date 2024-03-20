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
