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
    void sendOffline();
    void findReader() noexcept;
    int stateSession(char* state) noexcept;

    void forwarding();

    void checkReader();
    void answerClient(int statusCode);
    int answerCheck(char* answer);
    void clearMessageFromBufferUnconfirm(char* message);
    void setMessageStatus();
    
    int writeFile();
};