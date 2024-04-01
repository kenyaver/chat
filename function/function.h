#ifndef FUNCTION_H
#define FUNCTION_H

#include "aheader.h"

// конвертирует целочисленное значение, переданное во втором парамете, в строку, переданную в первом параметре
char* toString(char* str, int val);

// конвертирует строку, переданную в параметре в число (число возвращается функцией)
int fromString(const char* s);

// возвращает размер файла, путь до которого передан в параметре
int getFileSize(const char* fileName);

#endif