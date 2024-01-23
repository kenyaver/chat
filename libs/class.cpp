#include "class.h"



Client::Client() noexcept{}


// Client::Client(int sockfd){
//     this->status = 1;
//     this->sockfd = sockfd;
//     // reader = new Client();
//     // reader->status = 0;
// }

// Client::Client(const Client& a): Client(a.sockfd){}

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
    // std::cout << "close socket\n";
    // close(sockfd);
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

void Client::sendStateSession() noexcept{
    char state[12];
    if(this->reader->status == 0){
        strcpy(state, "offline\n");
    } else {
        strcpy(state, "online\n");
    }
    send(this->sockfd, state, sizeof(state), 0);
}

void Client::handleClient(){
    // std::thread t([&]{
        reader = new Client();
        this->status = 1;
        try{
            sendHelloClient();
        } catch(const char* errorMessage){
            std::cout << errorMessage << ": " << errno << '\n';
            // exit(EXIT_FAILURE);
            throw "client disconnected";
            this->~Client();
            return;
        }
        findReader();
        sendStateSession();

    // });
    // t.detach();
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

    delete reader; 
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