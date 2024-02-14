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
        this->sending(this->protocol.user, *this->protocol.partner);
        this->waitAnswer();
        // handle_answer();
        break;
        
        case 1:
        this->protocol.removeFromUnconfirm(this->protocol.user.buffrecv.header.messageID);
        this->sending(this->protocol.user, *this->protocol.partner);
        break;

        case 2:
        this->protocol.changePartner();
        this->protocol.addToUnconfirm();
        this->sending(this->protocol.user, *this->protocol.partner);
        this->waitAnswer();
        // handle_answer()
        break;

        case 3:
        this->sending(this->protocol.user, *this->protocol.partner);
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

void Session::sending(User& src, User& dst){
    src.buffrecv = src.buffsend;
    send(dst.sock, &src.buffsend, sizeof(src.buffsend), 0);
}

int Session::waitAnswer(){
    int MILLISECONDS = CLOCKS_PER_SEC / 1000;
    int end_time = clock() + 3000 * MILLISECONDS;

    struct timespec timeout;
    timeout.tv_sec = 1;
    struct pollfd fidesc;
    fidesc.fd = this->protocol.partner->sock;
    fidesc.events = POLLIN;

    while(clock() < end_time){
        int res = ppoll(&fidesc, 1, &timeout, NULL);

        if(res > 1){
            fidesc.revents = 0;
            this->recvPartner();
            if(this->handleAnswer()){
                return 1;
            }
        }
    }
    return 0;
}

void Session::recvPartner(){
    recv(this->protocol.partner->sock, &this->protocol.partner->buffrecv, sizeof(uint16_t), 0);
    this->protocol.partner->buffrecv.header.len = ntohs(this->protocol.partner->buffrecv.header.len);
    this->protocol.partner->buffrecv.message = new char[this->protocol.partner->buffrecv.header.len];
    recv(this->protocol.partner->sock, &this->protocol.partner->buffrecv + sizeof(this->protocol.partner->buffrecv.header.len), this->protocol.partner->buffrecv.header.len - sizeof(this->protocol.partner->buffrecv.header.len), 0);
}

int Session::handleAnswer(){
    this->sending(*this->protocol.partner, this->protocol.user);

    if(this->checkAnswer()){
        return 1;
    }

    return 0;
}

int Session::checkAnswer(){
    if(this->protocol.partner->buffrecv.header.type == 1){
        return 1;
    }
    return 0;
}

void Session::end(){
    for(auto i: this->protocol.unconfirm){
        this->protocol.saveUnconfirm(i);
    }
}