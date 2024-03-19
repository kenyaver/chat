#include "aheader.h"
#include "command.h"

class Unconfirm{
    std::queue<Command> unconfirm;
    public:
    void addToUnconfirm(Command& buffer);
    void removeFromUnconfirm();
    void saveUnconfirm(char* name);
};