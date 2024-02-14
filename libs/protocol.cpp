#include "protocol.h"

void Protocol::setUser(){
    memcpy(this->user.username, this->user.buffrecv.header.SRC, 8);
    this->user.status = 1;
    this->changePartner();
}

void Protocol::changePartner(){
    for(auto i: userDB){
        if(strcmp(i.username, this->user.buffrecv.header.DST) == 0){
            this->partner = &i;
            return;
        }
    }
    this->partner = new User;
    
    memcpy(this->partner->username, this->user.buffrecv.header.DST, 8);
    this->partner->sock = 0;
    this->partner->status = 0;
}

void Protocol::addToUnconfirm(){
    unconfirm.push_back(this->user.buffrecv);
}

void Protocol::removeFromUnconfirm(int messageID){
    unconfirm.erase(unconfirm.begin() + messageID);
}

void Protocol::saveUnconfirm(Command& offlineCommand){
    char* filename;
    sprintf(filename, "../offline/%s.txt", offlineCommand.header.DST);
    int file = open(filename, O_RDWR);
    write(file, &offlineCommand, sizeof(offlineCommand));
    close(file);
}