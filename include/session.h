#pragma once

#include "check.h"
#include "function.h"
#include "protocol.h"

class Session{
    public:
    Protocol protocol;
    Session(int sock);
    int worker();
};
