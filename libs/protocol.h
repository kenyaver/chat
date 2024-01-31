#pragma once
#include "check.h"
#include "function.h"
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
    
};