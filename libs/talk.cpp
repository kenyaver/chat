#include "talk.h"

void writeFile(Client clientic, char* user){
    char* nameFile = strcat(user, ".txt");
    char* pathFile = strcat("../offline", nameFile);
    char buffer[BUFFER_SIZE];
    FILE* file = fopen(pathFile, "a+");
    while(getFileSize(pathFile) < BUFFER_SIZE * 4){
        recv(clientic.sockfd, buffer, BUFFER_SIZE, 0);
        fprintf(file, buffer, BUFFER_SIZE);
    }
    send(clientic.sockfd, "Limit write file\n", 24, 0);
    close(clientic.sockfd);
}

void talk(Client client1, Client client2, std::atomic<bool>& flag){
    char bufferCheck1[BUFFER_SIZE * 4];
    char bufferCheck2[BUFFER_SIZE * 4];
    char bufferSend1[BUFFER_SIZE];
    char bufferSend2[BUFFER_SIZE];
    int id = 0;
    char idStr[4];
    time_t afk;
    struct pollfd fidesc;
    fidesc.fd = client1.sockfd;
    fidesc.events = POLLIN;
    int closer = 0;
    while(afk < 300000 && closer == 0 && flag == 0){
        int ret = poll(&fidesc, 1, 5000);
        if(ret == 0){
            afk += 5000;
            if(afk == 180000){
                send(client1.sockfd, "you innactive 3 minute!\n", 32, 0);
            }
            continue;
        }
        if(fidesc.revents && POLLIN){
            fidesc.revents = 0;
            recv(fidesc.fd, bufferSend1, BUFFER_SIZE, 0);
            if(exitClient(bufferSend1) == 1){
                closer = 1;
            }
            sprintf(idStr, "%d", id);
            char* buf = strcat(idStr, bufferSend1);
            strcat(bufferCheck1, buf);
            send(client2.sockfd, buf, 1024, 0);
            int ret = poll(&fidesc, 1, 10000);
            if(ret == 0){
                close(client2.sockfd);
                send(client1.sockfd, "partner innactive\n", 24, 0);
                break;
            }
            id++;
        }
    }
    flag.store(1);
}

void handleClient(Client clientic, char* user){
    if(userCheck(user, client) != 1){
        writeFile(clientic, user);
    } else {
        std::atomic<bool> flag{0};
        talk(clientic, findUser(user), flag);
    }
}