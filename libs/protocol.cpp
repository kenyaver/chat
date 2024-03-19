#include "protocol.h"

#include "check.h"
#include "function.h"

void protocol::addUser(User& user){
    online.addUser(user);
    // TODO: сделать проверку оффлан-команд.
}

void protocol::processRecvCommand(int sockfd, Command* buffer){
    recvCommand(sockfd, buffer);
}
