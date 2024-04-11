#include "../include/function.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>


char* toString(char* str, int val)
{
    sprintf(str, "%d", val);
    return str;
}

int fromString(const char* s) 
{
  int res = atoi(s);
  if(res == 0){
    res = -1;
  }
  return res;
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

void parseNames(const char* src, char* dst1, char* dst2){
    char sep[10] = " \0";
    char *isrc;

    isrc = strtok (const_cast<char*>(src), sep);
    strcpy(dst1, isrc);
        
    isrc = strtok (NULL, sep);
    strcpy(dst2, isrc);
}
