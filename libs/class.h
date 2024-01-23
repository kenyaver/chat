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

    Client() noexcept;
    Client(int sockfd) noexcept;
    Client(const Client& a) noexcept;
    Client(int sock, sockaddr *addr, socklen_t *addrLen);
    ~Client() noexcept;

    bool operator==(Client& a) noexcept;
    Client operator()() noexcept;
    void acceptClient(int sock, sockaddr_in addr);
    void handleClient();
    void sendHelloClient();
    void findReader() noexcept; // находит клиента с таким именем и присвает его указателю reader
    void sendStateSession() noexcept;
    // int readerStatus();
    int writeFile();
    int talk();
};

inline std::vector<Client> client;