#include "offline.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <cstdlib>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include "command.h"
#include "onlineList.h"


void Offline::setPath(char* username){
    sprintf(this->path, "../offlineTXT/%s.bin", username);
    std::cout << this->path << std::endl;
}

int Offline::readFile(Command* &buffer){
    std::ifstream reader(this->path, std::ios::binary | std::ios::in);
    if(!reader.is_open()){
        return 1;
    }
    while(!reader.eof()){
        reader.read((char*)&buffer->header, sizeof(Header));
        User* recver = onlineList.findUser(buffer->header.DST);
        buffer = (Command*)realloc(buffer, buffer->header.len);
        reader.read((char*)buffer->message, buffer->header.len - sizeof(Header));
        std::cout << "len: " << buffer->header.len
                << "\ntype: " << buffer->header.type
                << "\nmessageID: " << buffer->header.messageID
                << "\nsrc: " << buffer->header.SRC
                << "\ndst: " << buffer->header.DST << std::endl; 
        sendCommand(recver->sock, *buffer);
    }
    printf("success read\n");
    reader.close();
    remove(this->path);
    return 0;
}

int Offline::writeFile(Command& buffer){
    std::ofstream writer(this->path, std::ios::binary | std::ios::app);
    if(!writer.is_open()){
        writer.open(this->path, std::ios::binary | std::ios::out);
    }
    writer.write((char*)&buffer, buffer.header.len);
    writer.close();
    printf("success write\n");
    return 0;
}
