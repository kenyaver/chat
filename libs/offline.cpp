#include "offline.h"
#include "function.h"


void Offline::setPath(char* username){
    sprintf(this->path, "../offline/%s.txt", username);
}

bool Offline::checkFile(){
    if(getFileSize(this->path) == 0){
        return false;
    } else {
        return true;
    }
}
