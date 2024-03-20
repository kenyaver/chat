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
    // \param user пользователь, от которого принимается команда на сервер в bufferRecv
    // \note Функция будет обрабатывать пришедшую команду в соответствии с протоколом общения
    void processRecvCommand();

    // \param user пользователь, которому отправляется команда из bufferSend
    // \note Функция будет отправлять команду в соответствии с протоколом общения
    void processSendCommand();

    // \note очищение буффера неподтвержденных сообщений, запись их в файл, удаление пользователя из списка онлайн-пользователей, очищение очереди тайймеров
    void clearUser();
};
