#pragma once

#include <stdint.h>

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

// \return возвращает количество считанных байт
int recvCommand(int sockfd, Command* buffer);

// return возвращает количество отправленных байт
int sendCommand(int sockfd, Command& buffer);
