#pragma once

#include "offline.h"
#include "user.h"


class protocol{
    User* user;
    User* partner;
    OnlineList onlineList;
    public:
    void addUser(User& user);
    // \param sockfd сокет, по которому пришла команда
    // \param buffer буффер, в который будет записана команда
    // \note Функция будет обрабатывать пришедшую команду в соответствии с протоколом общения
    void processRecvCommand(User& user);

    // \param sockfd сокет, по которому пришла команда
    // \param buffer буффер, в который будет записана команда
    // \note Функция будет отправлять команду в соответствии с протоколом общения
    void processSendCommand(User& user);

    // \note очищение буффера неподтвержденных сообщений, запись их в файл, удаление пользователя из списка онлайн-пользователей, очищение очереди тайймеров
    void clearUser();
};
