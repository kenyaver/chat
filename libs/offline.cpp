#include "offline.h"

void Offline::setPath(char* username){
    sprintf(this->path, "../offline/%s.txt", username);
}

