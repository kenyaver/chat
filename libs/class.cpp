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



void Client::handleClient(){
    this->reader = new Client();
    this->status = 1;
    try{
        this->recvUsernames();
    } catch(const char* errorMessage){
        std::cout << errorMessage << ": " << errno << '\n';
        throw "client disconnected";
        memset(this, 0, sizeof(this));
        this->closeSocket();
        return;
    }

    char state[12];
    if(this->stateSession(state) == 0){
        send(this->sockfd, state, sizeof(state), 0);
        int i = 0;
        while(this->stateSession(state) == 0 && i < 4){
            try{
                this->recverOffline();
                i++;
                if(i == 4){
                    send(this->sockfd, "limit send to offline\n", 24, 0);
                }
            } catch(const char* errorMessage){
                throw errorMessage;
                break;
            }
        }
        
    } 

    if(this->stateSession(state) == 1){
        this->talk();
    }

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
    int ret = recv(sockfd, usernames, sizeof(usernames), MSG_WAITALL);
    if(ret > 0){
        parseNames(usernames, login, reader->login);
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



int Client::stateSession(char* state) noexcept{
    int res;

    this->findReader();
    if(this->reader->status == 0){
        strcpy(state, "offline\n");
        res = 0;
    } else {
        strcpy(state, "online\n");
        res = 1;
    }

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
    struct pollfd fidesc;               // создание структур для ppoll
    fidesc.fd = this->sockfd;
    fidesc.events = POLLIN;


    int afk = 0; // time of afk client in milliseconds
    char answer[1024];
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
            this->forwarding();
            if(keepAlive(this->reader->sockfd) == 0){
                recv(this->reader->sockfd, answer, sizeof(answer), 0);
                if(answerCheck(answer)){
                    this->answerClient(200);
                }
                clearMessageFromBufferUnconfirm(bufferRecv);
            } else {
                this->answerClient(300);
                this->recverOffline();
                break;
            }
        }
    }
}



void Client::answerClient(int statusCode){
    sprintf(bufferSend, "%d: %d", messageID, statusCode);
    send(this->sockfd, bufferSend, sizeof(bufferSend), 0);
}



int Client::answerCheck(char* answer){
    int answerInt = fromString(answer);
    if(answerInt != -1){
        if(answerInt == this->messageID){
            return 0;
        }
    }
    return 1;
}



void Client::forwarding(){
    recv(this->sockfd, bufferRecv, sizeof(bufferRecv), 0);
    this->checkReader();                                             // обработка и переотправка сообщений 
    for(int i = 0; i < 4; i++){
        if(bufferUnconfirm[i] == NULL){
            sprintf(bufferUnconfirm[i], "%s", bufferRecv);          // TODO сделать проверку на зполненность буффера
            break;
        }
    }
    send(this->reader->sockfd, bufferSend, sizeof(bufferSend), 0);
}



void Client::clearMessageFromBufferUnconfirm(char* message){
    char* find;
    for(int i = 0; i < 4; i++){
        find = strstr(bufferUnconfirm[i], message);
        
        if (find!=NULL) {
            char* find_ = find + strlen(message);
            strcpy (find, find_);
        }
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
    char *delim = (char*)" ";
    char* delimFlag = (char*)":";
    char* newReaderFlag = strtok(bufferRecv, delim);
    if(strstr(newReaderFlag, (char*)":") != NULL){
        char* newReader = strtok(newReaderFlag, delimFlag);
        strcpy(this->reader->login, newReader);
        this->findReader();
    }
}

void Client::sendOffline(){
    char *filename;
    sprintf(filename, "../offline/%s.txt", this->login);
    FILE* file = fopen(filename, "r");
    if(file){
        
    }
}