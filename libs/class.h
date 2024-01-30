#pragma once

#include "../libs/check.h"
#include "../libs/function.h"

class Client;

class Reader{
    public:
    char login[8];
    int sockfd;
    int status;
    char bufferRecv[1032]; // буффер для принятия сообщений
    char bufferSend[1032]; // буффер для отправки сообщений

    bool operator==(Reader& a) noexcept;
};




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

    bool operator==(Reader& a) noexcept;

    void acceptClient(int sock, sockaddr_in addr);

    void handleClient();
    
    
};




class Talk: public Client{
    char bufferUnconfirm[4][1032]; // буффер неподтвержденных сообщений

    
    int stateSession(char* state) noexcept;
    void sendOffline();
    int writeFile();
    void answerClient(int statusCode);
    int answerCheck(char* answer);
    void forwarding();
    void clearMessageFromBufferUnconfirm(char* message);
    void recverOffline();
    void checkReader();

    public:
    Talk();
    Talk(Talk& a);
    void talk();
    ~Talk();
};

inline std::vector<Client> client;