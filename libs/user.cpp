#include "user.h"

bool User::operator==(char* username){
    return !strcmp(this->username, username);
}

void User::clearQueueUnconfirm(){
    for(auto i = 0; i < this->unconfirm.size(); i++){
        this->unconfirm.pop();
    }
}

void User::disconnect(){
    shutdown(this->sock, SHUT_RDWR);
    close(this->sock);
    this->sock = -1;
    this->clearQueueUnconfirm();
    this->timer.clearTimerQueue();
}

void User::updateUserInfo(char* username){
    memcpy(this->username, username, 8);
}

void OnlineList::addUser(User& newUser){
    onlineList.push_back(newUser);
}

User* OnlineList::findUser(char* username){
    for(auto i = onlineList.begin(); i != onlineList.end(); i++){
        if(*i == username){
            return &*i;
        }
    }
    return NULL;
}

void OnlineList::removeUser(char* username){
    for(auto i = onlineList.begin(); i != onlineList.end(); i++){
        i->disconnect();
        onlineList.erase(i);
        return;
    }
}

void OnlineList::clearUserDB(){
    for(auto i = onlineList.begin(); i != onlineList.end(); i++){
        removeUser(i->username);       
    }
}
