#include "Client.h"


Client::Client() noexcept = default;



Client::Client(int sock){
    this->sockfd = sock;
}



Client::Client(const Client& a) noexcept{
    strcpy(this->username, a.username);
    this->sockfd = a.sockfd;
}



Client::~Client() noexcept{
    this->status = 0;
}



void Client::handleClient(){
    this->reader = new User();
    this->status = 1;
    try{
        this->recvUsernames();
        userDB.push_back(User(this->username, 1));
        userDB.push_back(User(this->reader->username, 0));
    } catch(const char* errorMessage){
        std::cout << errorMessage << ": " << errno << '\n';
        throw "client disconnected";
        memset(this, 0, sizeof(this));
        this->closeSocket();
        return;
    }

    this->findReader();

    this->talking();
    this->clearUser(this->reader->username);
    delete reader;
    this->closeSocket();
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