#include "Reader.h"

Reader::Reader() = default;

Reader::Reader(char* username){
    this->status = 1;
    strcpy(this->username, username);
}

bool Reader::operator==(Reader& a) noexcept{
    return !strcmp(this->username, a.username);
}

void Reader::clearReader(char* searchREader){
    for(auto i: readerDB){
        if(strcmp(i.username, searchREader) == 0){
            i.status = 0;
            memset(bufferRecv, 0, sizeof(bufferRecv));
            memset(bufferSend, 0, sizeof(bufferSend));
        }
    }
}

Reader::~Reader(){
    this->status = 0;
}