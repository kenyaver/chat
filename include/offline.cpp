#include "offline.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <cstdlib>


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

int Offline::readFile(Command* buffer){
    int file = open(this->path, O_RDONLY);
    int byte = read(file, &buffer->header, sizeof(buffer->header));
    buffer = (Command*)realloc(buffer, buffer->header.len);
    byte = read(file, buffer->message, buffer->header.len - sizeof(buffer->header));
    close(file);
    return byte;
}

int Offline::writeFile(Command& buffer){
    int file = open(this->path, O_RDONLY);
    int byte = write(file, &buffer, buffer.header.len);
    close(file);
    return byte;
}
