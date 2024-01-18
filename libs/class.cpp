#include "class.h"



Client::Client(){
    this->status = 1;
}

Client::Client(int sockfd){
    this->status = 1;
    this->sockfd = sockfd;
}

Client::Client(const Client& a): Client(a.sockfd){}

Client::~Client(){
    this->status = 0;
    close(sockfd);
}

void Client::helloClient(){
    char usernames[20];
    int ret = recv(client.back().sockfd, usernames, 20, 0);
    if(ret > 0){
        parse(usernames, login, reader);
        sprintf(bufferSend, "Hello %s!\n", login);
        int err = send(sockfd, bufferSend, sizeof(bufferSend), 0);
        bzero(bufferSend, sizeof(bufferSend));
        std::cout << err << '\n';
    }
}

int Client::readerStatus(){
    for(auto i: client){
        if(strcmp(i.login, this->reader) == 0){
            return i.status;
        }
    }
    return 0;
}

void Client::handleClient(){
    thr = std::thread([&]{
        this->helloClient();
        if(readerStatus() == 0){
            int i = 0;
            while(readerStatus() == 0 && i < 4){
                int ret = recv(sockfd, bufferRecv, sizeof(bufferRecv), 0);
                if(ret > 0){
                    writeFile();
                    i++;
                } else {
                    std::cout << errno << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        } else {
            std::cout << "talk\n";
            // talk();
        }
    });

    thr.detach();
    while(this->status == 1){}
    char bye[16];
    strcpy(bye, "bye!\n");
    send(sockfd, bye, sizeof(bye), 0);
}

int Client::writeFile(){
    char filename[32];
    sprintf(filename, "../offline/%s.txt", reader);
    FILE* file = fopen(filename, "a+");
    if(getFileSize(filename) < 4128){
        fprintf(file, "%s: %s", login, bufferSend);
    } else{
        std::cout << "file is FULL\n";
    }
    fclose(file);
    return getFileSize(filename);
}