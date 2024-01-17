#ifndef CLASS_H
#define CLASS_H

#include "../libs/aheader.h"
#include "../libs/check.h"
#include "../libs/function.h"

class Client{
    public:
    char login[8];
    char reader[8];
    int sockfd;
    int status;
    static int ID;
    int readerID;
    char bufferRecv[1024];
    char bufferSend[1024];
    std::thread thr;

    Client();

    Client(int sockfd);

    Client(const Client& a);
    
    ~Client();

    void HelloClient();

    int readerStatus();

    void handleClient();
};

std::vector<Client> client;

#endif