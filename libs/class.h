#pragma once

#include "../libs/check.h"
#include "../libs/function.h"

class Client{
    private:
    char bufferRecv[1032]; // буффер для принятия сообщений
    char bufferSend[1032]; // буффер для отправки сообщений
    char bufferUnconfirm[4][1024]; // буффер неподтвержденных сообщений
    char login[8]; // username клиента
    Client* reader; // указатель на клиента-получателя
    int status; // статус клиента
    int messageID; // идентификатор сообщения


    // принятие первого сообщения от клиента (логинов)
    void recvUsernames();

    void sendOffline();


    // отправляет клиенту-отправителю статус подключения клиента-получателя
    int stateSession(char* state) noexcept;


    // обработка и запись в файл сообщений для оффлайн-пользователя
    void recverOffline();

    // запись в файл
    int writeFile();

    // принятие сообщений, отправка команд клиенту-получателю и проверка подключения клиентов к серверу
    void talk();

    void answerClient(int statusCode);

    // проверка ответа от клиента
    int answerCheck(char* answer);

    // пересылает полученные сообщения партнеру
    void forwarding();

    // находит клиента с таким именем и присвает его указателю reader
    void findReader() noexcept;
    

    // очищает переданное сообщение из буффера неподтвержденных сообщений
    void clearMessageFromBufferUnconfirm(char* message);

    // проверяет сообщение на тег нового получателя
    void checkReader();
    
    // закрывает сокет и задает ему значение -1
    void closeSocket();

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
};

// контейнер всех клиентов
inline std::vector<Client> client;