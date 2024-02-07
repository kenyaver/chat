#include "user.h"

void User::disconnect(){
    close(this->sock);
    this->sock = -1;
    this->status = 0;
}