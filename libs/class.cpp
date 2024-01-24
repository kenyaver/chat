#include "class.h"

Client::Client() noexcept = default;

Client::Client(const Client& a) noexcept{
    strcpy(this->login, a.login);
    this->sockfd = a.sockfd;
}

Client::Client(int sock, sockaddr *addr, socklen_t *addrLen){
    try{
        this->sockfd = accept(sock, addr, addrLen);
    } catch(const char* errorMessage){
        throw errorMessage;
    }
}

Client::~Client() noexcept{
    this->status = 0;
}

bool Client::operator==(Client& a) noexcept{
    return !strcmp(this->login, a.login);
}

Client Client::operator()() noexcept{
    this->handleClient();
    return *this;
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

void Client::sendHelloClient(){
    char usernames[20];
    int ret = recv(sockfd, usernames, sizeof(usernames), MSG_WAITALL);
    if(ret > 0){
        parseNames(usernames, login, reader->login);
        char hello[32];
        sprintf(hello, "Hello %s!\n", login);
        int err = send(sockfd, hello, sizeof(hello), 0);
        std::cout << login << " connected\n";
    } else {
        throw "error recv usernames";
    }
}

void Client::findReader() noexcept{
    for(auto i: client){
        if(i == *this->reader){
            *reader = i;
        }
    }
}

int Client::sendStateSession() noexcept{
    char state[12];
    int res;
    if(this->reader->status == 0){
        strcpy(state, "offline\n");
        res = 0;
    } else {
        strcpy(state, "online\n");
        res = 1;
    }
    send(this->sockfd, state, sizeof(state), 0);

    return res;
}

void Client::closeSocket(){
    if(this->sockfd != -1){
        close(this->sockfd);
        this->sockfd = -1;
        std::cout << this->login << "`s socket successfully closed\n";
    } else{
        std::cout << "error: this socket was already closed\n";
    }
}

void Client::handleClient(){
    this->reader = new Client();
    this->status = 1;
    try{
        this->sendHelloClient();
    } catch(const char* errorMessage){
        std::cout << errorMessage << ": " << errno << '\n';
        // exit(EXIT_FAILURE);
        throw "client disconnected";
        this->~Client();
        return;
    }

    this->findReader();

    if(this->sendStateSession() == 0){
        int i = 0;
        while(i < 4){
            int ret = recv(this->sockfd, this->bufferRecv, sizeof(bufferRecv), 0);
            if(ret == -1){
                throw "error recv message for offline client\n";
                break;
            }
            this->writeFile();
            i++;
        }
        send(this->sockfd, "limit send to offline~\n", 24, 0);
    }

    delete reader;
    this->closeSocket();
}

int Client::writeFile(){
    char filename[32];
    sprintf(filename, "../offline/%s.txt", reader->login);
    FILE* file = fopen(filename, "a+");
    if(getFileSize(filename) < 4136){
        fprintf(file, "%s: %s", login, bufferSend);
    } else{
        std::cout << "file is FULL\n";
    }
    fclose(file);
    return getFileSize(filename);
}