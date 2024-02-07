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

    }
}
