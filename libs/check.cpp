#include "check.h"

int socketCheck(int domain, int type, int protocol){
    int opt = 1;
    socklen_t optLen = sizeof(opt);
    int sock = socket(domain, type, protocol);
    if(sock == -1){
        perror("socket failed\n");
        exit(EXIT_FAILURE);
   }
   setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &opt, optLen);
   return sock;
}

int bindCheck(int fd, struct sockaddr* addr, socklen_t lenAddr){
    int bin = bind(fd, addr, lenAddr);
    if(bin == -1){
        perror("bind failed\n");
        exit(EXIT_FAILURE);
    }
    return bin;
}

void listenCheck(int sockfd, int backlog){
    int res = listen(sockfd, 16);
    if(res == -1){
        perror("listen failed\n");
        // exit(EXIT_FAILURE);
    }
}

int acceptCheck(int sockfd, struct sockaddr* addr, socklen_t* lenAddr){
    int res = accept(sockfd, addr, lenAddr);
    if(res == -1){
        printf("accept failed\n");
        // exit(EXIT_FAILURE);
    }
    return res;
}

int connectCheck(int sockfd, const struct sockaddr* addr, socklen_t addrlen){
    int res = connect(sockfd, addr, addrlen);
    if(res == -1){
        perror("connect fail\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

int buffCheck(char* buffer){
    int size = strlen(buffer);
    if(size >= 512){
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
        // unknown error
        printf("poll error\n");
        return -2;
    }
    
    if(ret == 0){
        // client not connect
        return -1;
    }

    if(fidesc.revents && POLLIN){
        // client ok
        fidesc.revents = 0;
    }
    return 0;
}