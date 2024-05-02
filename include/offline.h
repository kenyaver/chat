#pragma once

#include "command.h"
#include <fstream>

class Offline{
    char path[29];
    public:
    void setPath(char* username);

    // \return возвращает существование записей в файле 
    bool checkFile();

    // читает сообщения из файла и отправляет получателю. Знаю, что плохо. Знаю, что очень..
    int readFile(Command* &buffer);

    // \return возвращает количество записанных байт в файл
    int writeFile(Command& buffer);
};
