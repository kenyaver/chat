#pragma once

#include "../libs/check.h"
#include "../libs/function.h"

class Client{
    private:
    char login[8];
    Client* reader;
    int status;
    char bufferRecv[1024];
    char bufferSend[1024];
    public:

    int sockfd;

    Client();
    Client(int sockfd);
    Client(const Client& a);
    ~Client();

    bool operator==(Client& a);
    Client operator()();
    void handleClient();
    void helloClient();
    void findReader();
    // int readerStatus();
    int writeFile();
    int talk();
};

inline std::vector<Client> client;



