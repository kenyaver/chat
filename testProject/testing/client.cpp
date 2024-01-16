#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1501);
    inet_pton(AF_INET, "192.168.10.92", &addr.sin_addr);
    socklen_t addrLen = sizeof(addr);
    connect(sock, (sockaddr*)&addr, addrLen);
    send(sock, argv[1], 8, 0);
    char buffer[1024];
    for(;;){
        int ret = recv(sock, buffer, 1024, 0);
        if(ret > 0){
            std::cout << buffer;
            if(strcmp(buffer, "bye!\n") == 0){
                break;
            }
        }
    }
    close(sock);
    return 0;
}