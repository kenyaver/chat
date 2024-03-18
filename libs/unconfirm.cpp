#include "unconfirm.h"

inline std::queue<Command> unconfirm;

void Unconfirm::addToUnconfirm(Command& buffer){
    unconfirm.push(buffer);
}

void Unconfirm::removeFromUnconfirm(){
    unconfirm.pop();
}

void Unconfirm::clearUnconfirm(char* name){
    char filePuth[24];
    sprintf(filePuth, "../offline/%s.txt", name);
    int file = open(filePuth, O_WRONLY);
    for(int i = 0; i < unconfirm.size(); i++){
        Command filer(unconfirm.front());
        unconfirm.pop();
        filer.header.type = 2;
        write(file, &filer, filer.header.len);
    }
}