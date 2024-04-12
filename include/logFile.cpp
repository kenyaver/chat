#include "logFile.h"
#include <mutex>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

std::mutex mut;

Logger::Logger(char* puth){
    this->fdLog = open(puth, O_WRONLY);
}

int Logger::writeLog(char* msg){
    mut.lock();
    write(this->fdLog, msg, strlen(msg) + 1);
    mut.unlock();
}

Logger::~Logger(){
    close(this->fdLog);
}