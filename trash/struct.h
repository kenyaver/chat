#ifndef STRUCT_H
#define STRUCT_H

#include "aheader.h"
#include "function.h"
#include "check.h"
#include "talk.h"

class Client{
    public:
    char login[8];
    int sockfd;
    int status;
    char buffer[1024];
    std::thread thr;

    Client();

    Client(int sockfd);

    Client(const Client& a);
    
    ~Client(){
        close(sockfd);
    }

    void handleClient(){
        thr = std::thread([&]{
        sprintf(buffer, "u cool %s!\n", this->login);
        int err = send(sockfd, buffer, 1024, 0);
        std::cout << err << '\n';
        });

        thr.join();
        char bye[16];
        strcpy(bye, "bye!\n");
        send(sockfd, bye, 16, 0);
        std::cout << "end session for client\n";
    }
};

int findUser(std::vector<Client>& client, char* recver);

#endif