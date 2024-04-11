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

int main(int argc, char* argv[]){
    if(strlen(argv[3]) < 9 && strlen(argv[4]) < 9){
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

        std::thread r([&]{
            while(1){
                
            }
        });

        int id = 0;
        while(1){
            
        }

        r.join();
        close(sock);
    }
    return 0;
}