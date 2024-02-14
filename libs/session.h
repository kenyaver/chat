#pragma once

#include "check.h"
#include "function.h"
#include "protocol.h"

class Session{
    private:
    Protocol protocol;
    void session();
    int checkOffline();
    void sendOffline();
    int readFile(char *filepath);
    void recving();
    void handleCommand();
    void sending(User& src, User& dst);
    int waitAnswer();
    void recvPartner();
    int handleAnswer();
    int checkAnswer();
    void end();
};