#include "loger.h"
#include <mutex>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

std::mutex muter;

Loger::Loger(char* puth){
    this->fdLog = open(puth, O_WRONLY);
}

int Loger::writeLog(char* msg){
    muter.lock();
    int byte = write(this->fdLog, msg, strlen(msg) + 1);
    muter.unlock();
    return byte;
}

Loger::~Loger(){
    close(this->fdLog);
}