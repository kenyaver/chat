#include "session.h"

void Session::session(){
    // initialization
    this->recving();
    this->protocol.setUser();
    if(this->checkOffline()){
        this->sendOffline();
    }

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

int Session::checkOffline(){
    char filepath[24];
    int exist;
    sprintf(filepath, "../offline/%s.txt", this->protocol.user.username);
    FILE* file = fopen(filepath, "r");
    if(file){
        exist = 1;
    } else {
        exist = 0;
    }
    fclose(file);
    return exist;
}

void Session::sendOffline(){
    char filepath[24];
    sprintf(filepath, "../offline/%s.txt", this->protocol.user.username);
    while(this->readFile(filepath)){
        send(this->protocol.user.sock, &this->protocol.user.buffsend, sizeof(this->protocol.user.buffsend), 0);
    }
}

int Session::readFile(char *filepath){
    int file = open(filepath, O_RDONLY);
    int bytes = read(file, &this->protocol.user.buffsend, sizeof(this->protocol.user.buffsend.header.len));
    bytes += read(file, &this->protocol.user.buffsend + sizeof(this->protocol.user.buffsend.header.len), this->protocol.user.buffsend.header.len - sizeof(this->protocol.user.buffsend.header.len));
    return bytes;
}

void Session::recving(){
    recv(this->protocol.user.sock, &this->protocol.user.buffrecv, sizeof(this->protocol.user.buffrecv.header.len), 0);
    this->protocol.user.buffrecv.header.len = ntohs(this->protocol.user.buffrecv.header.len);
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
    this->protocol.partner->buffrecv.message = new char[this->protocol.partner->buffrecv.header.len - 24];
    recv(this->protocol.partner->sock,
        &this->protocol.partner->buffrecv + sizeof(this->protocol.partner->buffrecv.header.len),
        this->protocol.partner->buffrecv.header.len - sizeof(this->protocol.partner->buffrecv.header.len), 0);
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