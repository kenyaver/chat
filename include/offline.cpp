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
    sprintf(this->path, "offlineTXT/%s.bin", username);
    write(STDIN_FILENO, this->path, 26);
    FILE* file;
}

int getFileSize(const char* fileName){
	int file_size = 0;
	struct stat fileStatbuff;
	int fd = open(fileName, O_RDONLY);
	if(fd == -1){
		file_size = -1;
	}
	else{
		if ((fstat(fd, &fileStatbuff) != 0) || (!S_ISREG(fileStatbuff.st_mode))) {
			file_size = -1;
		}
		else{
			file_size = fileStatbuff.st_size;
		}
		close(fd);
	}
	return file_size;
}

bool Offline::checkFile(){
    if(getFileSize(this->path) == 0){
        return false;
    } else {
        return true;
    }
}

int Offline::readFile(Command* &buffer){
    std::ifstream reader(this->path, std::ios::binary | std::ios::in);
    if(!reader.is_open()){
        return 1;
    }
    User* recver = onlineList.findUser(buffer->header.DST);
    while(!reader.eof()){
        reader.read((char*)&buffer->header, 24);
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
