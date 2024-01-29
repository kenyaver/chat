#pragma once

#include "../libs/check.h"
#include "../libs/function.h"

class Client{
    private:
    // принятие первого сообщения от клиента (логинов)
    void recvUsernames();

    // закрывает сокет и задает ему значение -1
    void closeSocket();

    protected:
    char bufferUnconfirm[4][1024]; // буффер неподтвержденных сообщений
    Client* reader; // указатель на клиента-получателя
    int messageID; // идентификатор сообщения

    // находит клиента с таким именем и присвает его указателю reader
    void findReader() noexcept;

    public:
    char login[8]; // username клиента
    int sockfd; // сокет для общения между клиентом и сервером
    int status; // статус клиента
    char bufferRecv[1032]; // буффер для принятия сообщений
    char bufferSend[1032]; // буффер для отправки сообщений

    // конструктор по умолчанию
    Client() noexcept;

    // конструктор для конструктора копирования
    Client(int sockfd) noexcept;

    // конструктор копирования
    Client(const Client& a) noexcept;

    // деструктор 
    ~Client() noexcept;

    // сравнивает логины клиентов
    bool operator==(Client& a) noexcept;

    // принятие клиента
    void acceptClient(int sock, sockaddr_in addr);

    // обработка клиента и подготовка к общению
    void handleClient();
};




class Talk: public Client{

    void sendOffline();
    int stateSession(char* state) noexcept;
    
    int writeFile();
    void answerClient(int statusCode);
    int answerCheck(char* answer);
    void forwarding();
    void clearMessageFromBufferUnconfirm(char* message);
    void recverOffline();
    void checkReader();

    public:
    Talk();
    Talk(Client& a);
    void talk();
    ~Talk();
};

// контейнер всех клиентов
inline std::vector<Client> client;