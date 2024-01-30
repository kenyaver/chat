#pragma once

#include "check.h"
#include "function.h"
#include "User.h"


class Session: public User{
    public:
    char bufferUnconfirm[4][1032]; // буффер неподтвержденных сообщений
    User* reader;
    int messageID;

    // находит клиента с таким именем и присвает его указателю reader
    void findReader() noexcept;
    int stateSession(char* state) noexcept;
    void sendOffline();
    int writeFile();
    void answerClient(int statusCode);
    int answerCheck(char* answer);
    void forwarding();
    void clearMessageFromBufferUnconfirm(char* message);
    void recverOffline();
    void checkReader();
    void talking(); // TODO: придумать нормальное название
    void talk();


    Session();
    Session(Session& a);
    // bool  operator==(Reader& a);
    ~Session();
};
