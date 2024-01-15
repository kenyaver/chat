#include "talk.h"

char* get(Client& client, char* buffer, int id){
    char bufferRecv[1024];
    recv(client.sockfd, bufferRecv, 1024, 0);
    char* strId = toString(strId, id);
    id++;
    strcat(buffer, strId);
    strcat(buffer, " ");
    strcat(buffer, bufferRecv);
    return buffer;
}

int put(char* buffer, Client& client){
    send(client.sockfd, buffer, 1028, 0);
    if(keepAlive(client.sockfd) != 0){
        close(client.sockfd);
        memcpy(client.login, "", 8);
        client.status = 0;
        return -1;
    }
    return 0;
}

void handleClient(std::vector<Client*>& clients, Client* client){
    char log_user[24];
    char recver[8];

    recv(client->sockfd, log_user, 20, 0);
    parse(log_user, client->login, recver);
    std::cout << client->login << " is trying to connect\n";
    send(client->sockfd, "check your username:\n", 1036, 0);

    if(loginCheck(client->login, clients) == -1){
        char usernameCheck[1036];
        memcpy(usernameCheck, client->login, 8);
        strcat(usernameCheck, " NOT OK\nchoose other username~\n");
        send(client->sockfd, usernameCheck, 1036, 0);
        close(client->sockfd);
        client->status = 0;
        return;
    } else {
        char usernameCheck[1036];
        memcpy(usernameCheck, client->login, 8);
        strcat(usernameCheck, " OK\nyou ready start message\n");
        send(client->sockfd, usernameCheck, 1036, 0);
    }

    send(client->sockfd, "search partner...\n", 1036, 0);
    Client tmp = findUser(clients, recver);
    std::vector<std::thread> t;
    char buffer[1036];
    char state[8];
    int size = 0;

    if(tmp.status == 0){
        memcpy(state, "offline\n", 8);
        send(client->sockfd, state, 1036, 0);
    }

    while(tmp.status == 0){
        tmp = findUser(clients, recver);
        if(size < 4128){
            recv(client->sockfd, buffer, 1024, 0);
            size = writeFile(client->login, recver, buffer);
        }
    }
    
    memcpy(state, "online\n", 8);
    send(client->sockfd, state, 1036, 0);

    int idUser = findIDuser(clients, recver);
    t.push_back(std::thread([&]{
        talk(client, clients.at(idUser));
    }));

    for(auto &i: t){
        i.join(); 
    }
    close(client->sockfd);
}

void talk(Client* client1, Client* client2){
    send(client1->sockfd, "start message\n", 16, 0);
    timespec timeout;
    timeout.tv_nsec = 5000;
    struct pollfd fidesc;
    fidesc.fd = client1->sockfd;
    fidesc.events = POLLIN;
    int afk = 0;
    int id = 0;
    char buffer[1028];
    while(afk != 300000 && exitClient(buffer) == 0 && client2->status == 1){
        int ret = ppoll(&fidesc, 1, &timeout, NULL);
        if(ret == 0){
            afk += 5000;
            if(afk == 180000){
                send(client1->sockfd, "you innactive!\n", 16, 0);
            }
        }
        if(fidesc.revents && POLLIN){
            std::cout << "client live\n";
            fidesc.revents = 0;
            client1->getData(id);
            if(client1->sendData(client2) != 0){
                char fileWriteBuffer[1036];
                strcat(fileWriteBuffer, client1->login);
                strcat(fileWriteBuffer, client1->bufferSend);
                writeFile(client1->login, client2->login, fileWriteBuffer);
                break;
            }
        }
    }
    return;
}

int writeFile(char* sender, char* recver, char* buffer){
    char* filename = strcat(recver, (char*)".txt");
    filename = strcat((char*)"../offline/", filename);
    int fileSize = getFileSize(filename);
    FILE* file;
    if(fileSize != 4096){
        file = fopen(filename, "a+");
    } else {
        return -1;
    }

    fprintf(file, "%s: %s", sender, buffer);
    return 0;
}