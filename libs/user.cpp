#include "user.h"

bool User::operator==(char* username){
    return !strcmp(this->username, username);
}

void User::disconnect(){
    close(this->sock);
    this->sock = -1;
}

void User::updateUserInfo(char* username){
    memcpy(this->username, username, 8);
}

void Online::addUser(User& newUser){
    online.push_back(newUser);
}

User Online::findUser(char* username){

}