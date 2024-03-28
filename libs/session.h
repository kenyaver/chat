#pragma once

#include "check.h"
#include "function.h"
#include "protocol.h"

class Session{
    private:
    Protocol protocol;
    Session(int sock);
    int waitFDs();
};
