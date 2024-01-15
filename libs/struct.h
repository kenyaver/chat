#ifndef STRUCT_H
#define STRUCT_H

#include "aheader.h"
#include "function.h"
#include "check.h"

class Client {
    public:
    char login[8];
    int sockfd;
    int status;
    char bufferRecv[1024];
    char bufferSend[1032];

    Client();
    Client(const char* login, int sockfd, int status, const char* bufferRecv, const char* bufferSend);
    Client(const Client& a);

    char* getData(int id);
    int sendData(Client* recver);

    ~Client();
};

char* getIPaddr(char* IPaddr);

void inet_ptonCheck(int af, char* src, void* dst);

struct sockaddr_in initAddrServer(int port);
struct sockaddr_in initAddrClient(int port, char* address);


Client findUser(std::vector<Client*>& client, char* user);
int findIDuser(std::vector<Client*>& client, char* login);

int loginCheck(char* log_us, std::vector<Client*>& client);
int userCheck(char* user, std::vector<Client*>& client);

#endif