#include "Client.h"


Client::Client() noexcept = default;



Client::Client(const Client& a) noexcept{
    strcpy(this->username, a.username);
    this->sockfd = a.sockfd;
}



Client::~Client() noexcept{
    this->status = 0;
}



void Client::handleClient(){
    this->reader = new Reader();
    this->status = 1;
    try{
        this->recvUsernames();
        readerDB.push_back(Reader(this->username));
    } catch(const char* errorMessage){
        std::cout << errorMessage << ": " << errno << '\n';
        throw "client disconnected";
        memset(this, 0, sizeof(this));
        this->closeSocket();
        return;
    }

    this->findReader();

    this->talking();
    this->clearReader(this->reader->username);
    delete reader;
    this->closeSocket();
}



void Client::acceptClient(int sock, sockaddr_in addr){
    try{
        socklen_t addrLen = sizeof(addr);
        this->sockfd = acceptCheck(sock, (sockaddr*)&addr, &addrLen);
        std::cout << "new client accepted\n";
    } catch(const char* errorMessage){
        throw errorMessage;
    }
}



void Client::recvUsernames(){
    char usernames[20];
    int ret = recv(sockfd, usernames, sizeof(usernames), 0);
    if(ret > 0){
        parseNames(usernames, username, reader->username);
        std::cout << username << " connected\n";
    } else {
        throw "error recv usernames";
    }
}







void Client::closeSocket(){
    if(this->sockfd != -1){
        close(this->sockfd);
        this->sockfd = -1;
        std::cout << this->username << "`s socket successfully closed\n";
    } else{
        std::cout << "error: this socket was already closed\n";
    }
}