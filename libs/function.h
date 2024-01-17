#ifndef FUNCTION_H
#define FUNCTION_H

#include "aheader.h"

char* toString(char* str, int val); // return int to char* (1 -> "1")
int fromString(const char* s); // return string to int ("1" -> 1)
char* getIPaddr(char* IPaddr); // return your IP-address
int getFileSize(const char* file_name); // return size of file
void parse(char* src, char* dst1, char* dst2); // set dst1 and dst2 from src 

#endif