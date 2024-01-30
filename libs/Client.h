#pragma once

#include "check.h"
#include "function.h"
#include "Session.h"

class Client: public Session{
    private:
    void recvUsernames();

    void closeSocket();

    public:

    Client() noexcept;

    Client(int sockfd) noexcept;

    Client(const Client& a) noexcept;

    ~Client() noexcept;

    void handleClient();
};

inline std::vector<Client> client;