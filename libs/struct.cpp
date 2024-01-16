#include "struct.h"

Client::Client(){
    bzero(login, 8);
    bzero(buffer, 1024);
}

Client::Client(int sockfd){
    this->sockfd = sockfd;
    status = 1;
}

Client::Client(const Client& a): Client(a.sockfd){
    std::cout << "copied\n";
}

Client::~Client(){
    close(sockfd);
}

void handleClient(){
    this->thr = std::thread([&]{
    sprintf(this->buffer, "u cool %s!\n", this->login);
    int err = send(sockfd, buffer, 1024, 0);
    std::cout << err << '\n';
    });

    thr.join();
    char bye[16];
    strcpy(bye, "bye!\n");
    send(sockfd, bye, 16, 0);
    std::cout << "end session for client\n";
}

int findUser(std::vector<Client>& client, char* recver){
    for(int i = 0; i < client.size(); i++){
        if(strcmp(client.at(i).login, recver) == 0){
            return i;
        }
    }
    return -1;
}