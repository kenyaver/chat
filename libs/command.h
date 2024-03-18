#pragma once

#include "aheader.h"

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
    char message[];
};

int recvCommand(int sockfd, Command& buffer);

int sendCommand(int sockfd, Command& buffer);