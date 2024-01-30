#include "User.h"

User::User() = default;

User::User(char* username, int status){
    this->status = status;
    strcpy(this->username, username);
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

User::~User(){
    this->status = 0;
}