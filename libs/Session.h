#pragma once

#include "protocol.h"

class Session{
    private:
    Protocol protocol;
    User* user;
    User* partner;
    int messageID;
    
    
    void setMod(); // -
    void online(); // -
    void offline(); // -
    void sendOffline();
    int stateSession(char* state) noexcept;
    void forwarding();
    public:
    void handleClient(); // +
    
    Session();
    Session(int sockfd);
    Session(Session& a);
    ~Session();
};