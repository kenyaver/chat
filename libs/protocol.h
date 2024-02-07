#pragma once

#include "check.h"
#include "function.h"
#include "user.h"



class Protocol{
    public:
    User user;
    User* partner;
    // std::map<uint32_t, char*> unconfirm;
    std::vector<Command> unconfirm;
    
    void setUser();
    void changePartner();
    void addToUnconfirm();
    void removeFromUnconfirm(int messageID);
    void saveUnconfirm(Command& offlineCommand);
};