#include "aheader.h"
#include "command.h"

class Unconfirm{
    public:
    void addToUnconfirm(Command& buffer);
    void removeFromUnconfirm();
    void clearUnconfirm(char* name);
};