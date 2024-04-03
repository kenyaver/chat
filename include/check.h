#ifndef CHECK_H
#define CHECK_H

#include <sys/socket.h>
#include <sys/types.h>

// создание сокета с проверкой на валидность
int socketCheck(int domain, int type, int protocol);

// создание TCP сокета (с парметрами AF_INET, SOCK_STREAM, protocol) с проверкой на валидность
int createTCPsocket(int protocol);

// присоединение сокета к адресу с проверкой на валидность
int bindCheck(int fd, struct sockaddr* addr);

// начало прослушивания порта ч проверкой на валидность
void listenCheck(int sockfd, int backlog);

// принятие клиента с проверкой на валидность
int acceptCheck(int sockfd, struct sockaddr* addr);

// подключение к серверу с проверкой на валидность
int connectCheck(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

// проверка сообщения на размер
int buffCheck(char* buffer);

// проверка на спец-символ в сообщении
int exitClient(char* buffer);

#endif
