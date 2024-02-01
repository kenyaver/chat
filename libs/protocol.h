#pragma once
#include "User.h"

class Protocol{
    public:
    std::map<int, char*> unconfirm; // буффер неподтвержденных сообщений

    void parseUsernames(char* usernames, char* usernameUser, char* usernamePartner);
    void findUser(User& user) noexcept;
    void setMessageStatus(char* message, int messageID, int statusCode);
    void appBufferUnconfirm(int messageID, char* message);
    int parseMessage(char* message);
   
    int checkUser(char* message);
    void answerClient(int statusCode);
    int answerCheck(char* answer);
    void clearMessageFromBufferUnconfirm(char* message);
    
    
    int writeFile();

    Protocol();
    ~Protocol();
};