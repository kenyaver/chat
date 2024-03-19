#pragma once

#include "command.h"

class Offline{
    char path[24];
    public:
    void setPath(char* username);
    bool checkFile();
    int readFile(Command* buffer);
    int writeFile(Command& buffer);
};
