#pragma once

#include "check.h"
#include "function.h"

class Header{
    public:
    uint16_t len;
    uint16_t type;
    uint32_t messageID;
    char SRC[8];
    char DST[8];
};

class Command{
    public:
    Header header;
    char *message;
};

class User{
    public:
    int sock;
    int status;
    char username[8];
    Command buffrecv;
    Command buffsend;
    void disconnect();
};

inline std::vector<User> userDB;