#pragma once

class Logger{
    int fdLog;

    public:
    Logger(char* puth);
    int writeLog(char* msg);
    ~Logger();
};