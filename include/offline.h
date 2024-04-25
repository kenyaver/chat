#pragma once

#include "command.h"
#include <fstream>

class Offline{
    char path[26];
    public:
    void setPath(char* username);

    // \return возвращает существование записей в файле 
    bool checkFile();

    // \return возвращает количество считанных байт из файла
    int readFile(Command* &buffer);

    // \return возвращает количество записанных байт в файл
    int writeFile(Command& buffer);
};
