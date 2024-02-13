#include "session.h"

void Session::session(){
    // initialization
    while(1){
        this->recving();
        this->handleCommand();
        // this->sending();
        // if(this->protocol.user.buffrecv.header.type == 0 || this->protocol.user.buffrecv.header.type == 2){
        //     this->waitAnswer();
        // }
    }
    this->end();
}

void Session::recving(){
    recv(this->protocol.user.sock, &this->protocol.user.buffrecv.header.len, sizeof(uint16_t), 0);
    this->protocol.user.buffrecv.header.len = ntohs(this->protocol.user.buffrecv.header.len);
    int len = this->protocol.user.buffrecv.header.len;
    this->protocol.user.buffrecv.message = (char*)malloc(this->protocol.user.buffrecv.header.len);
    recv(this->protocol.user.sock, &this->protocol.user.buffrecv + sizeof(this->protocol.user.buffrecv.header.len), this->protocol.user.buffrecv.header.len - sizeof(this->protocol.user.buffrecv.header.len), 0);
}

void Session::handleCommand(){
    switch(this->protocol.user.buffrecv.header.type){
        case 0:
        this->protocol.addToUnconfirm();
        this->sending();
        this->waitAnswer();
        break;
        
        case 1:
        this->protocol.removeFromUnconfirm(this->protocol.user.buffrecv.header.messageID);
        this->sending();
        break;

        case 2:
        this->protocol.changePartner();
        this->protocol.addToUnconfirm();
        this->sending();
        this->waitAnswer();
        break;

        case 3:
        this->sending();
        this->end();
        break;

        case 4:
        this->protocol.setUser();
        break;

        default:
        end();
        break;
    }
}

void Session::sending(){
    this->protocol.user.buffsend = this->protocol.user.buffsend;
    send(this->protocol.partner->sock, &this->protocol.user.buffsend, sizeof(protocol.user.buffsend), 0);
}

int Session::waitAnswer(){
    // TODO: сделать ppoll и таймер
}

void Session::end(){
    // TODO: сделать запись в файл всех обрабатываемых сообщений
}
