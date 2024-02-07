#pragma once

#include "check.h"
#include "function.h"
#include "protocol.h"

class Session{
    private:
    Protocol protocol;
    void session();
    void recving();
    void handleCommand();
    void sending();
    int waitAnswer();
    void end();
};