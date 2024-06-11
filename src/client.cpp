#include "../include/check.h"
#include "../include/command.h"
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <semaphore>
#include <condition_variable>
#include <algorithm>
#include <string>

Command *rCommand = (Command*)calloc(1, sizeof(Header));
Command *sCommand = (Command*)calloc(1, sizeof(Header));

void sendAnswer(int sock, Command* &command){
    std::swap(command->header.SRC, command->header.DST);
    command = (Command*)realloc(command, sizeof(command->header) + (4 * sizeof(char)));
    strcpy(command->message, "200");
    command->header.type = 1;
    sendCommand(sock, *command);
    command = NULL;
}

void printCommand(Command* command){
    std::cout << "\rfrom: " << command->header.SRC << std::endl;
    if(command->header.type == 1){
        std::cout << "answer for ID: " << command->header.messageID << std::endl;
    }
    std::cout << "message: " << command->message << std::endl;
    std::cout << std::endl;
    command = NULL;
}

int setCommand(std::string& buffer, std::string& src, std::string& dst, int id, Command* &command){
    command = (Command*)realloc(command, sizeof(Header) + buffer.size());
    strcpy(command->message, buffer.c_str());
    command->header.len = sizeof(command->header) + buffer.size();
    command->header.messageID = id;
    memcpy(command->header.SRC, src.c_str(), 8);
    memcpy(command->header.DST, dst.c_str(), 8);
    command->header.type = 0;
    id++;
    return id;
}

int checkExit(std::string& buffer){
    if(buffer.find("~") != std::string::npos){
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]){
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(atoi(argv[2]));
        inet_pton(AF_INET, argv[1], &addr.sin_addr);
        socklen_t addrLen = sizeof(addr);

        int err = connect(sock, (sockaddr*)&addr, addrLen);
        if(err == -1){
            std::cout << "connect error: " << errno << '\n';
            exit(EXIT_FAILURE);
        }
        std::cout << "connected to server" << std::endl;
        
        std::string src;
        std::cout << "SRC: ";
        std::getline(std::cin, src);
        if(src.size() == 0 || src.size() > 8){
            std::cout << "bad name" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::binary_semaphore newMessageWrited(0);
        std::binary_semaphore newMessageReaded(1);
        std::binary_semaphore worker(0);
        std::condition_variable work;
        
        std::thread r([&]{
            while(!worker.try_acquire()){
                newMessageReaded.acquire();
                int byte = recvCommand(sock, rCommand);
                switch (byte){
                    case -1:
                        std::cerr << "error: " << errno << std::endl;
                        worker.release();
                        break;
                    case 0:
                        std::cerr << "server died" << std::endl;
                        worker.release();
                        break;
                    default:
                        if(rCommand->header.type == 0){
                            sendAnswer(sock, rCommand);
                        }
                        newMessageWrited.release();
                        break;
                }
            }
        });

        int id = 0;
        std::cout << "for quit enter \'~\' in message" << std::endl;
        while(!worker.try_acquire()){
            std::string dst;
            std::cout << "DST: ";
            std::getline(std::cin, dst);
            if(dst.size() == 0 || dst.size() > 8){
                if(newMessageWrited.try_acquire()){
                    printCommand(rCommand);
                    newMessageReaded.release();
                }
                continue;
            }
            std::string buffer;
            std::cout << "message: ";   
            std::getline(std::cin, buffer);
            if(buffer.size() == 0 || buffer.size() > 1000){
                std::cout << "bad message" << std::endl;
                if(newMessageWrited.try_acquire()){
                    printCommand(rCommand);
                    newMessageReaded.release();
                }
                continue;
            }
            if(checkExit(buffer)){
                worker.release();
                if(newMessageWrited.try_acquire()){
                    printCommand(rCommand);
                }
                break;
            }
            if(newMessageWrited.try_acquire()){
                printCommand(rCommand);
                newMessageReaded.release();
            }
            id = setCommand(buffer, src, dst, id, sCommand);
            sendCommand(sock, *sCommand);
            sCommand = NULL;
        }

        r.join();
        close(sock);
        free(rCommand);
        free(sCommand);
    return 0;
}
