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

// \param sockfd сокет, с которого нужно считать пришедшую команду
// \param buffer буффер команды, в которую будет записана пришедшая на сокет команда
// \return возвращает количество считанных байт
// \note в случае возврата -1, следует обрабатывать errno-значение
int recvCommand(int sockfd, Command* buffer);

// \param sockfd сокет, на который нужно отправить команду
// \param buffer буффер команды из которой нужно считать данные для отправки по сокету
// return возвращает количество отправленных байт
int sendCommand(int sockfd, Command& buffer);
