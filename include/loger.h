#pragma once

class Loger{
    int fdLog;

    public:
    Loger(char* puth);
    int writeLog(char* msg);
    ~Loger();
};

inline Loger log = Loger((char*)"log.txt");