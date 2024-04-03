#include "check.h"
#include <sys/poll.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>


int socketCheck(int domain, int type, int protocol){
    int sock = socket(domain, type, protocol);
    if(sock == -1){
        // perror("socket failed\n");
        // std::cout << errno << '\n';
        // exit(EXIT_FAILURE);
        throw "socket failed";
   }
   return sock;
}

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


int buffCheck(char* buffer){
    int size = strlen(buffer);
    if(size >= 1024){
        return -1;
    }
    return 0;
}


int exitClient(char* buffer){
    int size = strlen(buffer);
    for(int i = 0; i < size; i++){
        if(buffer[i] == '~'){
            return 1;
        }
    }
    return 0;
}


int keepAlive(int sock){
    struct timespec timeout;
    timeout.tv_sec = 10;
    struct pollfd fidesc;
    fidesc.fd = sock;
    fidesc.events = POLLIN;

    int ret = ppoll(&fidesc, 1, &timeout, NULL);
    if(ret == -1){
        printf("poll error: %d\n", errno);
        return -2;
    }
    
    if(ret == 0){
        return -1;
    }

    if(fidesc.revents && POLLIN){
        fidesc.revents = 0;
    }
    return 0;
}
