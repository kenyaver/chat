#include "class.h"

Client::Client(){
    this->ID++;
}

Client::Client(int sockfd){
    this->sockfd = sockfd;
}

Client::Client(const Client& a): Client(a.sockfd){}

Client::~Client(){
    close(sockfd);
}

void Client::HelloClient(){
    char usernames[20];
    int ret = recv(client.back().sockfd, usernames, 20, 0);
    if(ret > 0){
        parse(usernames, login, reader);
        sprintf(bufferRecv, "Hello %s!\n", login);
        int err = send(sockfd, bufferRecv, 1024, 0);
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
        this->HelloClient();
        if(this->readerStatus() == 0){
            // writeFile();
            std::cout << "writeFile\n";
        } else {
            std::cout << "talk\n";
            // talk();
        }
    });

    thr.join();
    char bye[16];
    strcpy(bye, "bye!\n");
    send(sockfd, bye, 16, 0);
}