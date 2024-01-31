#include "protocol.h"

void Protocol::setMessageStatus(){
    sprintf(message, "%d: %d", messageID, statusCode);
}

void Protocol::recvUsernames(){
    char usernames[20];
    int ret = recv(this->user->sockfd, usernames, sizeof(usernames), 0);
    if(ret > 0){
        parseNames(usernames, this->user->username, reader->username);
        std::cout << this->user->username << " connected\n";
    } else {
        throw "error recv usernames";
    }
}

void Protocol::answerClient(int statusCode){
    sprintf(this->user->bufferSend, "%d: %d", messageID, statusCode);
    send(this->user->sockfd, this->user->bufferSend, sizeof(this->user->bufferSend), 0);
}

int Protocol::answerCheck(char* answer){
    int answerInt = fromString(answer);
    if(answerInt != -1){
        if(answerInt == this->messageID){
            return 0;
        }
    }
    return 1;
}