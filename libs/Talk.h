#pragma once

#include "check.h"
#include "function.h"
#include "Client.h"

class Talk: public Client{
    char bufferUnconfirm[4][1032]; // буффер неподтвержденных сообщений

    
    int stateSession(char* state) noexcept;
    void sendOffline();
    int writeFile();
    void answerClient(int statusCode);
    int answerCheck(char* answer);
    void forwarding();
    void clearMessageFromBufferUnconfirm(char* message);
    void recverOffline();
    void checkReader();

    public:
    Talk();
    Talk(Talk& a);
    void talk();
    ~Talk();
};
