#include "onlineList.h"
#include <mutex>

static std::mutex mut;

void OnlineList::addUser(User& newUser){
    mut.lock();
    onlineList.push_back(newUser);
    mut.unlock();
}

User* OnlineList::findUser(char* username){
    mut.lock();
    for(auto i = onlineList.begin(); i != onlineList.end(); i++){
        if(*i == username){
            mut.unlock();
            return &*i;
        }
    }
    mut.unlock();
    return NULL;
}

void OnlineList::removeUser(char* username){
    mut.lock();
    for(auto i = onlineList.begin(); i != onlineList.end(); i++){
        i->disconnect();
        onlineList.erase(i);
        mut.unlock();
        return;
    }
    mut.unlock();
}

void OnlineList::clearUserDB(){
    mut.lock();
    for(auto i = onlineList.begin(); i != onlineList.end(); i++){
        removeUser(i->username);       
    }
    mut.unlock();
}
