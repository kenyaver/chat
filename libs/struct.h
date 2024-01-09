#ifndef STRUCT_H
#define STRUCT_H

#include "aheader.h"

typedef struct {
char login[8];
int *sockfd;
int status;
} Client;

char* getIPaddr(char* IPaddr);

void inet_ptonCheck(int af, char* src, void* dst);

struct sockaddr_in initAddrServer(int af, int port);
struct sockaddr_in initAddrClient(int af, int port, char* address);


Client findUser(std::vector<Client>& client, char* user);
int findIDuser(std::vector<Client>& client, char* login);

int loginCheck(char* log_us, std::vector<Client>& client, int id);
int userCheck(char* user, std::vector<Client>& client);

#endif