#pragma once

#include "../libs/check.h"
#include "../libs/function.h"

class Client{
    private:
    char login[8];
    char reader[8];
    int status;
    char bufferRecv[1024];
    char bufferSend[1024];
    std::thread thr;
    public:

    int sockfd;

    Client();

    Client(int sockfd);

    Client(const Client& a);
    
    ~Client();

    void helloClient();

    int readerStatus();

    void handleClient();
};

inline std::vector<Client> client;