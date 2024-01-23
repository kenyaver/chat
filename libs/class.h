#pragma once

#include "../libs/check.h"
#include "../libs/function.h"

class Client{
    private:
    char bufferRecv[1024];
    char bufferSend[1024];
    char login[8];
    Client* reader;
    int status;
    public:

    int sockfd;

    Client();
    Client(int sockfd);
    Client(const Client& a);
    Client(int sock, sockaddr *addr, socklen_t *addrLen);
    ~Client();

    bool operator==(Client& a);
    Client operator()();
    void handleClient();
    void helloClient();
    void findReader() noexcept;
    void acceptClient(int sock, sockaddr_in addr);
    // int readerStatus();
    int writeFile();
    int talk();
};

inline std::vector<Client> client;