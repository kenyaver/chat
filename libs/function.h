#ifndef FUNCTION_H
#define FUNCTION_H

#include "aheader.h"

char* toString(char* str, int val);

int fromString(const char* s);

int getFileSize(const char* file_name);

void parse(char* src, char* dst1, char* dst2);

#endif