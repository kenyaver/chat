#include "onlineList.h"

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