#pragma once
#include "command.h"
#include <queue>
#include <cstddef>

class Unconfirm{
    std::queue<Command> unconfirm;

    public:
    void push(Command& buffer);
    void pop();
    Command* front();
    size_t size();
};