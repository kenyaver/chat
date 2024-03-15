#pragma once

#include "check.h"
#include "function.h"
#include "protocol.h"

class Session{
    private:
    Protocol protocol;
    Session();
    int waitFDs();
    void handleCommand();
    void handleTimer();
    
    int sending();
    int recving();

    void end();
};