#ifndef STRUCT_H
#define STRUCT_H

#include "aheader.h"
#include "function.h"
#include "check.h"

/class Client{
    public:
    char login[8];
    int sockfd;
    int status;
    char buffer[1024];
    std::thread thr;

    Client();

    Client(int sockfd);

    Client(const Client& a): Client(a.sockfd){
        // std::cout << "copied\n";
    }
    
    ~Client();

    void handleClient(std::vector<Client>& client);
};

// char* getIPaddr(char* IPaddr);

// void inet_ptonCheck(int af, char* src, void* dst);

// struct sockaddr_in initAddrServer(int port);
// struct sockaddr_in initAddrClient(int port, char* address);


Client findUser(std::vector<Client*>& client, char* user);
int findIDuser(std::vector<Client*>& client, char* login);

int userCheckStatus(std::vector<Client*>& client, char* user);

#endif