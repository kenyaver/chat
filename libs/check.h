#ifndef CHECK_H
#define CHECK_H

#include "aheader.h"

// создание сокета
int socketCheck(int domain, int type, int protocol);

// присоединение сокета к адресу
int bindCheck(int fd, struct sockaddr* addr, socklen_t lenAddr);

// начало прослушивания порта
void listenCheck(int sockfd, int backlog);

// принятие клиента
int acceptCheck(int sockfd, struct sockaddr* addr, socklen_t* lenAddr);

// подключение к серверу
int connectCheck(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

// проверка сообщения на размер
int buffCheck(char* buffer);

// проверка на спец-символ в сообщении
int exitClient(char* buffer);

// проверка состояния подключения
int keepAlive(int sock);

#endif