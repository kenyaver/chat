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
    in >> buffer.header.len
    >> buffer.header.type
    >> buffer.header.messageID
    >> buffer.header.SRC
    >> buffer.header.DST;
}

int Offline::readFile(Command* &buffer){
    // int file = open(this->path, O_RDONLY);
    // int byte = read(file, &buffer->header, sizeof(buffer->header));
    // buffer = (Command*)realloc(buffer, buffer->header.len);
    // byte = read(file, buffer->message, buffer->header.len - sizeof(buffer->header));
    // close(file);
    std::ifstream reader(this->path, std::ios::in);
    readHeader(reader, *buffer);
    buffer = (Command*)realloc(buffer, buffer->header.len);
    reader >> buffer->message;
    reader.close();
    return 0;
}

int Offline::writeFile(Command& buffer){
    // int file = open(this->path, O_WRONLY);
    // int byte = write(file, &buffer, buffer.header.len);
    // close(file);
    std::ofstream writer(this->path, std::ios::app);
    if(!writer.is_open()){
        writer.close();
        writer.open(this->path, std::ios::out);
    }
    writer << buffer.header.len
        << buffer.header.type
        << buffer.header.messageID
        << buffer.header.SRC
        << buffer.header.DST
        << buffer.message;
    writer.close();
    return 0;
}
