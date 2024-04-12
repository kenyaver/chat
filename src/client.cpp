#include "../include/check.h"
#include "../include/command.h"
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <algorithm>
#include <string>

#define BUFFERrSIZE 1024
#define BUFFERsSIZE 1024

void sendAnswer(int sock, Command* command){
    std::swap(command->header.SRC, command->header.DST);
    command = (Command*)realloc(command,sizeof(command->header) + 4 * sizeof(char));
    memcpy(command->message, "200", 4);
    command->header.type = 1;
    sendCommand(sock, *command);
}

void printCommand(Command* command){
    std::cout << "from: " << command->header.SRC << std::endl;
    if(command->header.type == 1){
        std::cout << "answer for ID: " << command->header.messageID << std::endl;
    }
    std::cout << "\nmessage: " << command->message << std::endl;
}

void setCommand(std::string& buffer, std::string& src, std::string& dst, int* id, Command* command){
    command = (Command*)realloc(command, sizeof(command->header) + buffer.size() + 1);
    memcpy(command->message, buffer.c_str(), buffer.size() + 1);
    command->header.len = sizeof(command->header) + buffer.size() + 1;
    command->header.messageID = *id;
    *id += 1;
    memcpy(command->header.SRC, src.c_str(), 8);
    memcpy(command->header.DST, dst.c_str(), 8);
    command->header.type = 0;
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
            err = connect(sock, (sockaddr*)&addr, addrLen);
            std::cout << "connect error: " << errno << '\n';
            exit(EXIT_FAILURE);
        }
        std::cout << "connected to server\n";
        
        std::string src;
        std::cout << "your name: ";
        std::getline(std::cin, src);
        if(src.size() == 0 || src.size() > 7){
            std::cout << "bad name" << std::endl;
            exit(1);
        }

        std::atomic<int> work{0};
        Command *rCommand = (Command*)calloc(24, sizeof(char));
        Command *sCommand = (Command*)calloc(24, sizeof(char));
        std::thread r([&]{
            int byte;
            struct pollfd pfd;
            pfd.fd = sock;
            pfd.events = POLLIN | POLLRDHUP | POLLHUP;

            while(work.load() == 0){
                int ret = poll(&pfd, 1, 3000);
                if(ret < 0){
                    std::cerr << "error: " << errno << std::endl;
                    work.store(1);
                    return;
                }
                if(ret == 0){
                    continue;
                }

                if(ret > 0){
                    if(pfd.revents == POLLIN){
                        byte = recvCommand(sock, rCommand);
                        if(byte != -1){
                            printCommand(rCommand);
                        if(rCommand->header.type == 0){
                            sendAnswer(sock, rCommand);
                        }
                        } else {
                            std::cout << "error message" << std::endl;
                            work.store(1);
                        }
                    } else {
                        std::cout << "server died" << std::endl;
                        pfd.revents = 0;
                        work.store(1);
                    }
                }
            }
            return;
        });

        int id = 0;
        while(work.load() == 0){
            std::string dst;
            std::cout << "input DST:\n";
            std::getline(std::cin, dst);
            if(dst.size() == 0 || dst.size() > 7){
                std::cout << "bad DST" << std::endl;
                continue;
            }
            std::string buffer;
            std::cout << "input message:\n";   
            std::getline(std::cin, buffer);
            if(buffer.size() == 0 || buffer.size() > 999){
                std::cout << "bad message" << std::endl;
                continue;
            }
            setCommand(buffer, src, dst, &id, sCommand);
            sendCommand(sock, *sCommand);
            
        }

        r.join();
        close(sock);
    return 0;
}
