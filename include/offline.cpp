#include "offline.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <cstdlib>
#include <sys/stat.h>


void Offline::setPath(char* username){
    sprintf(this->path, "../offlineTXT/%s.txt", username);
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
    int file = open(this->path, O_RDONLY);
    int byte = read(file, &buffer->header, sizeof(buffer->header));
    buffer = (Command*)realloc(buffer, buffer->header.len);
    byte = read(file, buffer->message, buffer->header.len - sizeof(buffer->header));
    close(file);
    return byte;
}

int Offline::writeFile(Command& buffer){
    int file = open(this->path, O_WRONLY);
    int byte = write(file, &buffer, buffer.header.len);
    close(file);
    return byte;
}
