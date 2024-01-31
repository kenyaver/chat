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

void User::clearUser(char* searchReader){
    for(auto i: userDB){
        if(strcmp(i.username, searchReader) == 0){
            i.status = 0;
            memset(bufferRecv, 0, sizeof(bufferRecv));
            memset(bufferSend, 0, sizeof(bufferSend));
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