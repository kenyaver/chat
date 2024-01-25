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

    this->findReader();
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

    if(this->sendStateSession() == 0){
        int i = 0;
        while(this->sendStateSession() == 0 && i < 4){
            try{
                    this->recverOffline();
                i++;
                if(i == 4){
                    send(this->sockfd, "limit send to offline~\n", 24, 0);
                }
            } catch(const char* errorMessage){
                throw errorMessage;
                return;
            }
            
        }
    } else {
        std::cout << "talk...\n";
    }

    delete reader;
    this->closeSocket();
}



int Client::writeFile(){
    char filename[32];
    sprintf(filename, "../offline/%s.txt", reader->login);
    if(getFileSize(filename) < 4136){
        FILE* file = fopen(filename, "a+");
        fprintf(file, "%s: %s\n", login, bufferRecv);
        fclose(file);
    } else{
        throw "file is FULL";
    }
    return getFileSize(filename);
}



void Client::talk(){
    struct timespec timeout;
    timeout.tv_sec = 10;
    struct pollfd fidesc;
    fidesc.fd = this->sockfd;
    fidesc.events = POLLIN;
    int afk = 0; // time of afk client in milliseconds
    int id = 0;
    char answer[4];
    while(afk < 300000){

        int res = ppoll(&fidesc, 1, &timeout, NULL);
        if(res == 0){
            afk += 10000;
            if(afk == 180000){
                send(this->sockfd, "you innactive!\n", 16, 0);
            }
        }

        if(res == -1){
            throw "ppoll failed";
        }

        if(res > 0){
            fidesc.revents = 0;
            afk = 0;
            recv(this->sockfd, bufferRecv, sizeof(bufferRecv), 0);
            this->checkReader();
            this->setCommand(id);
            id++;                                                                   // обработка и переотправка сообщений 
            strcat(bufferUnconfirm, bufferRecv);
            send(this->reader->sockfd, bufferSend, sizeof(bufferSend), 0);
            if(keepAlive(this->reader->sockfd) == 0){
                recv(this->reader->sockfd, answer, sizeof(answer), 0);
                clearMessageFromBufferUnconfirm(bufferRecv);
            } else {
                this->recverOffline();
                break;
            }
        }
    }
}



void Client::setCommand(int id){
    char id_str[4];
    toString(id_str, id);
    sprintf(bufferSend, "%s %s: %s", id_str, this->login, bufferRecv);
}



void Client::clearMessageFromBufferUnconfirm(char* message){
    char* find = strstr(bufferUnconfirm, message);

    if (find!=NULL) {
        char* find_ = find + strlen(message);
        strcpy (find, find_);
    }
}



void Client::recverOffline(){
    int ret = recv(this->sockfd, this->bufferRecv, sizeof(bufferRecv), 0);
    if(ret == -1){
        throw "error recv message for offline client\n";
        return;
    }
    try{
        this->writeFile();
    } catch(const char* errorMessage){
        std::cout << errorMessage << std::endl;
        return;
    }
}



void Client::checkReader(){
        char *delim = " ";
        char* delimFlag = ":";
        char* newReaderFlag = strtok(bufferRecv, delim);
        if(strstr(newReaderFlag, ":") != NULL){
            char* newReader = strtok(newReaderFlag, delimFlag);
            strcpy(this->reader->login, newReader);
            this->findReader();
        }
    }