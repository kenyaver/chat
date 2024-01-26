#ifndef FUNCTION_H
#define FUNCTION_H

#include "aheader.h"

// конвертирует целочисленное значение, переданное во втором парамете, в строку, переданную в первом параметре
char* toString(char* str, int val);

// конвертирует строку, переданную в параметре в число (число возвращается функцией)
int fromString(const char* s);

// возвращает размер файла, путь до которого передан в параметре
int getFileSize(const char* fileName);

// парсит src в dst1 и dst2 по пробелу (src{"Artem artem"} -> dst1{Artem}, dst2{artem})
void parseNames(const char* src, char* dst1, char* dst2); // set dst1 and dst2 from src

#endif