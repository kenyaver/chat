#pragma once
#include "User.h"

class Protocol{
    protected:
    User* user;
    User* reader;
    int statusCode;
    char* message;
    char bufferUnconfirm[4][1032]; // буффер неподтвержденных сообщений
    int messageID;

    void recvUsernames();
    void setMessageStatus();
    void answerClient(int statusCode);
    int answerCheck(char* answer);
    void sendOffline();
    int stateSession(char* state) noexcept;
    void checkReader();
    void forwarding();
    void findReader() noexcept;
    void clearMessageFromBufferUnconfirm(char* message);
    
};