#pragma once

#include "check.h"
#include "function.h"
#include "Reader.h"
#include "Talk.h"

class Client: public Talk{
    private:
    void recvUsernames();

    void closeSocket();

    protected:

    

    public:

    Client() noexcept;

    Client(int sockfd) noexcept;

    Client(const Client& a) noexcept;

    ~Client() noexcept;

    void acceptClient(int sock, sockaddr_in addr);

    void handleClient();
    
};

inline std::vector<Client> client;