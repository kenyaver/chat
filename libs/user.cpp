#include "user.h"

void User::disconnect(){
    close(this->sock);
    this->sock = -1;
}

void UserDB::addUser(User& newUser){
    userDB.push_back(newUser);
}