#include "user.h"

bool User::operator==(char* username){
    return !strcmp(this->username, username);
}

void User::disconnect(){
    shutdown(this->sock, SHUT_RDWR);
    close(this->sock);
    this->sock = -1;
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

void Online::removeUser(char* username){
    for(auto i = online.begin(); i != online.end(); i++){
        i->disconnect();
        online.erase(i);
        return;
    }
}

void Online::clearUserDB(){
    for(auto i = online.begin(); i != online.end(); i++){
        removeUser(i->username);       
    }
}
