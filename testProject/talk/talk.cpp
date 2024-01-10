#include "../../libs/talk.h"

char* get(char* buffer, Client& client, int id){
    char bufferRecv[1024];
    recv(*client.sockfd, bufferRecv, 1024, 0);
    char* strId = toString(strId, id);
    id++;
    strcat(buffer, strId);
    strcat(buffer, " ");
    strcat(buffer, bufferRecv);
    return buffer;
}

int put(char* buffer, Client& client){
    send(*client.sockfd, buffer, 1028, 0);
    if(keepAlive(client.sockfd) != 0){
    // send(*client1.sockfd, "partner innactive\n", 24, 0);
        close(*client.sockfd);
        return -1;
    }
    return 0;
}

void talk(Client&& client1, Client&& client2){
    std::cout << "hh\n";
    send(*client1.sockfd, "start message\n", 16, 0);
    struct pollfd fidesc1;
    fidesc1.fd = *client1.sockfd;
    fidesc1.events = POLLIN;
    int afk = 0;
    int id = 0;
    char buffer[1028];
    std::cout << "hmmm..\n";
    while(afk != 300000 && exitClient(buffer) == 0){
        int ret = poll(&fidesc1, 1, 5000);
        if(ret == 0){
            afk += 5000;
            if(afk == 180000){
                send(*client1.sockfd, "you innactive!\n", 16, 0);
            }
        }
        if(fidesc1.revents && POLLIN){
            std::cout << "client live\n";
            fidesc1.revents = 0;
            get(buffer, client1, id);
            if(put(buffer, client2) != 0){
                char fileWriteBuffer[1036];
                strcat(fileWriteBuffer, client1.login);
                strcat(fileWriteBuffer, buffer);
                writeFile(client1.login, client2.login, fileWriteBuffer);
            }
        }
    }
}

int writeFile(char* sender, char* recver, char* buffer){
    char* filename = strcat(recver, (char*)".txt");
    filename = strcat((char*)"../offline/", filename);
    int fileSize = getFileSize(filename);
    FILE* file;
    if(fileSize != 4128){
        file = fopen(filename, "a+");
    } else {
        return -1;
    }

    fprintf(file, "%s: %s", sender, buffer);
    return 0;
}