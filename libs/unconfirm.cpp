#include "unconfirm.h"

int Unconfirm::size(){
    return this->unconfirm.size();
}

void Unconfirm::addToUnconfirm(Command& buffer){
    unconfirm.push(buffer);
}

void Unconfirm::removeFromUnconfirm(){
    unconfirm.pop();
}

Command Unconfirm::getCommand(){
    return this->unconfirm.front();
}