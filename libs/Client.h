#pragma once

#include "check.h"
#include "function.h"
#include "Reader.h"
#include "Talk.h"

class Client: public Reader{
    private:
    void recvUsernames();

    void closeSocket();

    protected:
    Reader* reader; // указатель на клиента-получателя
    int messageID; // идентификатор сообщения

    // находит клиента с таким именем и присвает его указателю reader
    void findReader() noexcept;

    public:

    Client() noexcept;

    Client(int sockfd) noexcept;

    Client(const Client& a) noexcept;

    ~Client() noexcept;

    // bool operator==(Reader& a) noexcept;

    void acceptClient(int sock, sockaddr_in addr);

    void handleClient();
    
    
};

inline std::vector<Client> client;