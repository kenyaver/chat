#pragma once

#include "protocol.h"

class Session: public Protocol{
    public: 

    // находит клиента с таким именем и присвает его указателю reader
    void handleClient();
    void setMod();
    void online();
    void offline();

    Session();
    Session(int sockfd);
    Session(Session& a);
    ~Session();
};