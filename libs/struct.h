#ifndef STRUCT_H
#define STRUCT_H

#include "aheader.h"
#include "check.h"

typedef struct {
int sockfd;
char login[8];
int status;
} Client;

void inet_ptonCheck(int af, char* src, void* dst);

struct sockaddr_in initAddrServer(int sf, int port);
struct sockaddr_in initAddrClient(int af, int port, char* address);


Client findUser(Client* client, char* user);

int loginCheck(char* log_us, Client* client, int id);
int userCheck(char* user, Client* client);

#endif