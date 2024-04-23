#include "offline.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <cstdlib>
#include <sys/stat.h>
#include <fstream>


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

void readHeader(std::ifstream& in, Command& buffer){
    // in >> buffer.header.len
    // >> buffer.header.type
    // >> buffer.header.messageID
    // >> buffer.header.SRC
    // >> buffer.header.DST;
    in.read((char*)&buffer.header, 24);
}

int Offline::readFile(Command* &buffer){
    std::ifstream reader(this->path, std::ios::binary | std::ios::in);
    if(reader.is_open()){
        while(!reader.eof()){
            reader.read((char*)&buffer->header, 24);
            buffer = (Command*)realloc(buffer, buffer->header.len);
            reader.read((char*)buffer->message, buffer->header.len - sizeof(Header));
            reader.close();
            remove(this->path);
        }
    } else {
        return 1;
    }
    return 0;
}

int Offline::writeFile(Command& buffer){
    std::ofstream writer(this->path, std::ios::binary | std::ios::app);
    if(!writer.is_open()){
        writer.close();
        writer.open(this->path, std::ios::binary | std::ios::out);
    }
    writer.write((char*)&buffer, buffer.header.len);
    writer.close();
    return 0;
}
