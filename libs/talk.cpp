#include "talk.h"

void talk(Client& client1, Client& client2){
    send(*client1.sockfd, "start message\n", 16, 0);
    struct pollfd fidesc1;
    fidesc1.fd = *client1.sockfd;
    fidesc1.events = POLLIN;
    int afk = 0;
    int id = 0;
    char buffer[4096];
    char bufferRecv[1024];
    char bufferSend[1028];
    while(afk != 300000 && exitClient(bufferRecv) == 0 && exitClient(bufferSend) == 0){
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
                char fileWriteBuffer[1024];
                strcat(fileWriteBuffer, client1.login);
                strcat(fileWriteBuffer, bufferSend);
                writeFile(client1.login, client2.login, fileWriteBuffer);
            }
        }
    }
}

int writeFile(char* sender, char* recver, char* buffer){
    char* filename = strcat(recver, (char*)".txt");
    filename = strcat((char*)"../offlene/", filename);
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


