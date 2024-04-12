#pragma once

#include "protocol.h"


class Session{
    public:
    Protocol protocol;
    Session(int sock);
    void worker();
};
