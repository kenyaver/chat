#pragma once

#include "offline.h"
#include "user.h"


class Protocol{
    User* user;
    User* partner;
    OnlineList onlineList;
    Offline offline;
    public:
    void addUser(User& user);
    // \note Функция будет обрабатывать пришедшую команду в соответствии с протоколом общения
    void processRecvCommand();

    // \note Функция будет отправлять команду в соответствии с протоколом общения
    void processSendCommand();

    // \note очищение буффера неподтвержденных сообщений, запись их в файл, удаление пользователя из списка онлайн-пользователей, очищение очереди таймеров
    void clearUser();
};
