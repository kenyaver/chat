#include "User.h"

User::User() = default;

User::User(char* username, int sockfd,  int status){
    strcpy(this->username, username);
    this->sockfd = sockfd;
    this->status = status;
}

User::User(User& a): User{a.username, a.sockfd, a.status}{}

User::User(char* username, int status){
    strcpy(this->username, username);
    this->status = status;
}

bool User::operator==(User& a) noexcept{
    return !strcmp(this->username, a.username);
}

void User::disconnectUser(char* searchReader){
    for(auto i: userDB){
        if(strcmp(i.username, searchReader) == 0){
            i.status = 0;
            memset(this->bufferRecv, 0, sizeof(this->bufferRecv));
            memset(this->bufferSend, 0, sizeof(this->bufferSend));
        }
    }
}

void User::closeSocket(){
    close(this->sockfd);
    this->sockfd = -1;
    this->status = 0;
}

User::~User(){
    this->status = 0;
}