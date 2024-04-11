#include "../include/check.h"
#include "../include/function.h"
#include "../include/command.h"
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <thread>

#define BUFFERrSIZE 1032
#define BUFFERsSize 1032

char* getMessage(char* message, char* messageID){
    int i = 0;
    while(message[i] != ' '){
        messageID[i] = message[i];
    }
    if(fromString(messageID) != -1){
        return messageID;
    }
    else{
        return (char*)"not answer";
    }
}

char* returnID(char* id_str, int id){
    return toString(id_str, id);
}

int main(int argc, char* argv[]){
    if(strlen(argv[3]) < 9 && strlen(argv[4]) < 9){
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(fromString(argv[2]));
        inet_pton(AF_INET, argv[1], &addr.sin_addr);
        socklen_t addrLen = sizeof(addr);

        int err = connect(sock, (sockaddr*)&addr, addrLen);
        if(err == -1){
            err = connect(sock, (sockaddr*)&addr, addrLen);
            std::cout << "connect error: " << errno << '\n';
            exit(EXIT_FAILURE);
        }
        std::cout << "connected to server\n";

        char usernames[20];
        sprintf(usernames, "%s %s", argv[3], argv[4]);
        err = send(sock, usernames, sizeof(usernames), 0);
        if(err == -1){
            std::cout << errno << '\n';
            close(sock);
            exit(EXIT_FAILURE);
        }
        std::cout << "usernames: " << err << " bytes\n";



        char state[12];
        err = recv(sock, state, sizeof(state), MSG_WAITALL);
        if(err < 1){
            std::cout << errno << '\n';
            close(sock);
            exit(EXIT_FAILURE);
        }
        std:: cout << state << std::endl;

        char bufferR[BUFFERrSIZE];
        char bufferS[BUFFERsSize];
        char* id_str;
        std::thread r([&]{
            while(exitClient(bufferR) == 0 && exitClient(bufferS) == 0){
                int ret = recv(sock, bufferR, sizeof(bufferR), 0);
                
                if(ret == 0){
                    std::cout << errno << std::endl; 
                    continue;
                }
                if(ret > 0){
                    id_str = getMessage(bufferR, id_str);
                    if(strcmp(id_str, "not answer") != 0){
                        send(sock, id_str, sizeof(id_str), 0);
                    }
                    std::cout << bufferR << std::endl;
                }
                if(ret == -1){
                    std::cout << errno << '\n';
                }
            }
        });

        std::thread s([&]{
            int id = 0;
            while(exitClient(bufferR) == 0 && exitClient(bufferS) == 0){
                char buffer[BUFFERsSize];
                std::cin.getline(buffer, BUFFERsSize);
                if(strlen(buffer) == BUFFERsSize){
                    std::cout << "error message\n";
                    break;
                } else {
                    sprintf(bufferS, "%d %s", id, buffer);
                    std::cout << bufferS << std::endl;
                    send(sock, bufferS, sizeof(bufferS), 0);
                    bzero(bufferS, sizeof(bufferS));
                    bzero(buffer, sizeof(buffer));
                    id++;
                }
            }
        });

        r.join();
        s.join();
        close(sock);
    } else {
        std::cout << "error names\n";
    }
    return 0;
}