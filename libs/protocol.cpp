#include "protocol.h"

#include "check.h"
#include "function.h"

void protocol::addUser(User& user){
    onlineList.addUser(user);
    this->user = onlineList.findUser(user.username);
}

void protocol::processRecvCommand(User& user){
    
}
