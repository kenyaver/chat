#pragma once

#include "command.h"
#include "function.h"

class Offline{
    char path[24];
    public:

    // \param username используется для создания пути
    void setPath(char* username);

    // \return возвращает существование записей в файле 
    bool checkFile();

    // \param buffer используется для записи в буффер из файла
    // \return возвращает количество считанных байт из файла
    int readFile(Command* buffer);
    
    // \param buffer используется для записи в файл из буффера
    // \return возвращает количество записанных байт в файл
    int writeFile(Command& buffer);
};
