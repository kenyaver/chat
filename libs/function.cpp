#include "function.h"

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

int getFileSize(const char* file_name){
	int file_size = 0;
	struct stat fileStatbuff;
	int fd = open(file_name, O_RDONLY);
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

void parse(char* src, char* dst1, char* dst2){
    int size = strlen(src);
    int flagdst = 0;
    int j = 0;
    int k = 0;
    for(int i = 0; i < size; i++){
        if(src[i] == ' '){
            flagdst++;
            continue;
        }
        if(flagdst == 0){
            dst1[j] = src[i];
            j++;
        } else {
            dst2[k] = src[i];
            k++;
        }
    }
}