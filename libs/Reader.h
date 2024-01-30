#pragma once

#include "check.h"
#include "function.h"

class Reader{
    public:
    Reader();
    Reader(char* username);
    char username[8];
    int sockfd;
    int status;
    char bufferRecv[1032]; // буффер для принятия сообщений
    char bufferSend[1032]; // буффер для отправки сообщений

    bool operator==(Reader& a) noexcept;

    void clearReader(char* searchReader);

    ~Reader();
};

inline std::vector<Reader> readerDB;