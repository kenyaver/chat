#pragma once

#include "check.h"
#include "function.h"
#include "command.h"

class User{
    public:
    int sock;
    int status;
    char username[8];
    Command buffrecv;
    Command buffsend;
    void disconnect();
};

class UserDB{
    std::vector<User> userDB;

    public:
    
};

