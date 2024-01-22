#include "class.h"



Client::Client(){
    this->sockfd = 0;
    this->status = 1;
}

Client::Client(int sockfd){
    this->status = 1;
    this->sockfd = sockfd;
    // reader = new Client();
    // reader->status = 0;
}

Client::Client(const Client& a): Client(a.sockfd){}

Client::~Client(){
    this->status = 0;
    close(sockfd);
    // delete reader;
}

bool Client::operator==(Client& a){
    return !strcmp(this->login, a.login);
}

Client Client::operator()(){
    this->handleClient();
    return *this;
}

void Client::helloClient(){
    char usernames[20];
    int ret = recv(sockfd, usernames, sizeof(usernames), MSG_WAITALL);
    if(ret > 0){
        parse(usernames, login, reader->login);
        std::cout << login << ' ' << reader->login << '\n';
        char hello[32];
        sprintf(hello, "Hello %s!\n", login);
        int err = send(sockfd, hello, sizeof(hello), 0);
        std::cout << login << " connected\n";
        std::cout << err << ' ' << errno << '\n';
    } else {
        std::cout << "error recv usernames\n";
        std::cout << errno << '\n';
        std::cout << this->sockfd << '\n';
        exit(EXIT_FAILURE);
    }
}

void Client::findReader(){
    for(auto i: client){
        if(i == *this->reader){
            *reader = i;
        }
    }
}

void Client::handleClient(){
    std::thread t([&]{
        this->status = 1;
        helloClient();
        findReader();
        std::cout << reader->status << '\n';
    });
    t.detach();
        // if(reader->status == 0){
        //     int i = 0;
        //     while(reader->status == 0 && i < 4){
        //         send(sockfd, "offline\n", 12, 0);
        //         int ret = recv(sockfd, bufferRecv, sizeof(bufferRecv), 0);
        //         if(ret > 0){
        //             writeFile();
        //             i++;
        //         } else {
        //             std::cout << errno << std::endl;
        //             exit(EXIT_FAILURE);
        //         }
        //     }
        // } else {
        //     send(sockfd, "online\n", 12, 0);
        //     std::cout << "talk\n";
        //     // talk();
        // }
    // char bye[16];
    // strcpy(bye, "bye!\n");
    // send(sockfd, bye, sizeof(bye), 0);
}

int Client::writeFile(){
    char filename[32];
    sprintf(filename, "../offline/%s.txt", reader->login);
    FILE* file = fopen(filename, "a+");
    if(getFileSize(filename) < 4128){
        fprintf(file, "%s: %s", login, bufferSend);
    } else{
        std::cout << "file is FULL\n";
    }
    fclose(file);
    return getFileSize(filename);
}