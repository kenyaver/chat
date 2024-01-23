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
    // сокет для общения между клиентом и сервером
    int sockfd;

    // конструктор по умолчанию
    Client() noexcept;

    // конструктор для конструктора копирования
    Client(int sockfd) noexcept;

    // конструктор копирования
    Client(const Client& a) noexcept;

    // неудачная попытка принимать клиента при создании объекта
    Client(int sock, sockaddr *addr, socklen_t *addrLen);

    // деструктор 
    ~Client() noexcept;

    // сравнивает логины клиентов
    bool operator==(Client& a) noexcept;

    // пока что без предназначения
    Client operator()() noexcept;

    // принятие клиента
    void acceptClient(int sock, sockaddr_in addr);

    // обработка клиента и подготовка к общению
    void handleClient();

    // принятие первого сообщения от клиента (логинов) и отправка приветствия клиенту
    void sendHelloClient();

    // находит клиента с таким именем и присвает его указателю reader
    void findReader() noexcept;

    // отправляет клиенту-отправителю статус подключения клиента-получателя
    void sendStateSession() noexcept;


    // запись в файл полученных сообщений
    int writeFile();

    // принятие сообщений, отправка команд клиенту-получателю и проверка подключения клиентов к серверу
    int talk();
};

// контейнер всех клиентов
inline std::vector<Client> client;