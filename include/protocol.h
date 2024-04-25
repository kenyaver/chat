#pragma once

#include "offline.h"
#include "onlineList.h"
#include "user.h"


class Protocol{
    // OnlineList onlineList;
    // Offline offline;
    public:
    User* user;
    User* partner;
    
    void addUser(User& user);
    void handleCommand();
    int helloUser();
    void processSendCommand();
    void handleTimer();
    // \note очищение буффера неподтвержденных сообщений, запись их в файл, удаление пользователя из списка онлайн-пользователей, очищение очереди таймеров
    void clearUser();
};
