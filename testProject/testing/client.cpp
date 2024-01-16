#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <vector>


int main(int argc, char* argv[]){
    if(strlen(argv[1]) < 9 && strlen(argv[2]) < 9){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(1500);
        inet_pton(AF_INET, "192.168.10.92", &addr.sin_addr);
        socklen_t addrLen = sizeof(addr);
        int err = connect(sock, (sockaddr*)&addr, addrLen);
        char usernames[20];
        sprintf(usernames, "%s %s", argv[1], argv[2]);
        send(sock, usernames, 20, 0);
        char buffer[1024];
        for(;;){
            std::thread r([&]{
                for(;;){
                    char buffer[1024];
                    int ret = recv(sock, buffer, 1024, 0);
                    if(ret > 0){
                        std::cout << buffer;
                    } else {
                        std::cout << errno << std::endl;
                        break;
                    }
                }
            });

            std::thread s([&]{
                for(;;){
                    char buffer[1025];
                    std::cin.getline(buffer, 1025);
                    if(strlen(buffer) == 1025){
                        std::cout << "error message\n";
                        break;
                    } else {
                        send(sock, buffer, 1024, 0);
                    }
                }
            });
        }
        close(sock);
    }
    return 0;
}