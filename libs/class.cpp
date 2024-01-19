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

bool Client::operator==(Client& a){
    return !strcmp(this->login, a.login) && this->sockfd == a.sockfd && this->status == a.status;
}

Client Client::operator()(){
    this->handleClient();
    return *this;
}

void Client::helloClient(){
    char usernames[20];
    int ret = recv(client.back().sockfd, usernames, 20, 0);
    if(ret > 0){
        parse(usernames, login, reader->login);
        char hello[32];
        sprintf(hello, "Hello %s!\n", login);
        int err = send(sockfd, hello, sizeof(hello), 0);
        std::cout << login << " connected\n";
        // bzero(bufferSend, sizeof(bufferSend));
        std::cout << err << ' ' << errno << '\n';
    }
}

int Client::readerStatus(){
    for(auto i: client){
        if(i == *this->reader){
            return i.status;
        }
    }
    return 0;
}

void Client::handleClient(){
        helloClient();
        if(readerStatus() == 0){
            int i = 0;
            while(readerStatus() == 0 && i < 4){
                send(sockfd, "offline\n", 12, 0);
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
            send(sockfd, "online\n", 12, 0);
            std::cout << "talk\n";
            // talk();
        }
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