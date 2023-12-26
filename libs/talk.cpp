#include "talk.h"

void handleClient(std::vector<Client>&& clients, Client&& clientic, char* user){
    Client partner = findUser(clients, user);
    if (partner.status == 1){
        std::atomic<int> flag{0};
        std::thread t(talk, std::move(clientic), std::move(partner), std::move(flag));
        t.join();
    } else {
        std::thread w(writeFile, std::move(clientic), partner.login);
        w.join();
    }
}

void talk(Client&& client1, Client&& client2, std::atomic<int>&& flag){
    struct pollfd fidesc1;
    fidesc1.fd = *client1.sockfd;
    fidesc1.events = POLLIN;
    int afk = 0;
    int id = 0;
    char buffer[4096];
    char bufferRecv[1024];
    char bufferSend[1028];
    while(afk != 300000 && flag.load() != 1){
        int ret = poll(&fidesc1, 1, 10000);
        if(ret == 0){
            afk += 10000;
            if(afk == 180000){
                send(*client1.sockfd, "you innactive!\n", 16, 0);
            }
        }
        if(fidesc1.revents && POLLIN){
            fidesc1.revents = 0;
            char* strId = toString(strId, id);
            recv(*client1.sockfd, bufferRecv, 1024, 0);
            strcat(bufferSend, strId);
            strcat(bufferSend, bufferRecv);
            send(*client2.sockfd, bufferSend, 1028, 0);
            if(keepAlive(client2.sockfd) != 0){
                send(*client1.sockfd, "partner innactive\n", 24, 0);
                close(*client2.sockfd);
                writeFile(std::move(client1), client2.login);
            }
        }
    }
}

void writeFile(Client&& clientic, char* user){
    struct pollfd fidesc1;
    fidesc1.fd = *clientic.sockfd;
    fidesc1.events = POLLIN;
    int afk = 0; // ms innactive
    char bufferRecv[1024];
    char* filename = strcat(user, (char*)".txt");
    filename = strcat((char*)"../offlene/", filename);
    int fileSize = getFileSize(filename);
    while(afk != 300000 && fileSize != 4096){
        FILE* file = fopen(filename, "a+");
        int ret = poll(&fidesc1, 1, 10000);
        if(ret == 0){
            afk += 10000;
            if(afk == 180000){
                send(*clientic.sockfd, "you innactive!\n", 16, 0);
            }
        }
        if(fidesc1.revents && POLLIN){
            fidesc1.revents = 0;
            recv(*clientic.sockfd, bufferRecv, 1024, 0);
            fprintf(file, "%s: %s", clientic.login, bufferRecv);
            fclose(file);
            fileSize = getFileSize(filename);
        }
    }
}


