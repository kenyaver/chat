#pragma once

#include "check.h"
#include "function.h"
#include "Reader.h"
#include "Client.h"

class Talk: public Reader{
    public:
    char bufferUnconfirm[4][1032]; // буффер неподтвержденных сообщений
    Reader* reader;
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


    Talk();
    Talk(Talk& a);
    // bool  operator==(Reader& a);
    ~Talk();
};
