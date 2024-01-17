#ifndef CHECK_H
#define CHECK_H

#include "aheader.h"


int socketCheck(int domain, int type, int protocol);
int bindCheck(int fd, struct sockaddr* addr, socklen_t lenAddr);
void listenCheck(int sockfd, int backlog);
int acceptCheck(int sockfd, struct sockaddr* addr, socklen_t* lenAddr);
int connectCheck(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
int buffCheck(char* buffer);
int exitClient(char* buffer);
int keepAlive(int sock);

#endif