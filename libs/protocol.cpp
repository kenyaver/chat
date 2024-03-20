#include "protocol.h"

#include "check.h"
#include "function.h"

void Protocol::addUser(User& user){
    onlineList.addUser(user);
    this->user = onlineList.findUser(user.username);
}

void Protocol::processRecvCommand(User& user){
    
}
