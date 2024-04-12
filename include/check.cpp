#include "check.h"
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int createTCPsocket(int protocol){
    int sock = socket(AF_INET, SOCK_STREAM, protocol);
    if(sock == -1){
        throw "socket failed";
    }
    return sock;
}

int bindCheck(int fd, struct sockaddr* addr){
    socklen_t addrLen = sizeof(*addr);
    int err = bind(fd, addr, addrLen);
    if(err == -1){
        throw "bind failed";
    }
    return err;
}

void listenCheck(int sockfd, int backlog){
    int res = listen(sockfd, 16);
    if(res == -1){
        throw "listen failed";
    }
}

int acceptCheck(int sockfd, struct sockaddr* addr){
    socklen_t addrLen = sizeof(*addr);
    int res = accept(sockfd, addr, &addrLen);
    if(res == -1){
        throw "accept failed";
    }
    return res;
}

int connectCheck(int sockfd, const struct sockaddr* addr, socklen_t addrlen){
    int res = connect(sockfd, addr, addrlen);
    if(res == -1){
        throw "connect failed\n";
    }
    return res;
}
