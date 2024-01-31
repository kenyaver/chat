#include "Session.h"
#include "Client.h"

void Session::handleClient(){
    this->reader = new User();
    this->user->status = 1;
    try{
        this->recvUsernames();
        userDB.push_back(User(this->reader->username, 0));
    } catch(const char* errorMessage){
        std::cout << errorMessage << ": " << errno << '\n';
        throw "client disconnected";
        memset(this, 0, sizeof(this));
        this->user->closeSocket();
        return;
    }

    this->findReader();

    this->setMod();

    this->reader->disconnectUser(this->reader->username);
    delete reader;
    this->user->closeSocket();
}


void Session::setMod(){
    char state[12];
    int sessionRes = this->stateSession(state);
    send(this->user->sockfd, state, sizeof(state), 0);
    this->sendOffline();
    if(sessionRes == 1){
        this->online();
    } else{
        int i = 0;
        while(i < 4 && getFileSize(this->reader->username) < 4128 && this->stateSession(state) == 0){
            this->offline();
        }
        if(i == 4 || getFileSize(this->reader->username) >= 4128){
            send(this->user->sockfd, "limit send to offline client~\n", 32, 0);
            return;
        }
    }
    this->online();
}

void Session::online(){
    struct timespec timeout;
    timeout.tv_sec = 10;
    struct pollfd fidesc;               // создание структур для ppoll
    fidesc.fd = this->user->sockfd;
    fidesc.events = POLLIN;

    int afk = 0; // time of afk client in milliseconds
    
    while(afk < 300000){

        int res = ppoll(&fidesc, 1, &timeout, NULL);
        if(res == 0){
            afk += 10000;
            if(afk == 180000){
                send(this->user->sockfd, "you innactive!\n", 16, 0);
            }
        }

        if(res == -1){
            throw "ppoll failed";
        }

        if(res > 0){
            fidesc.revents = 0;
            afk = 0;

            this->forwarding();
            char answer[1032];
            if(keepAlive(this->reader->sockfd) == 0){
                recv(this->reader->sockfd, answer, sizeof(answer), 0);

                if(answerCheck(answer)){
                    this->answerClient(200);
                    clearMessageFromBufferUnconfirm(answer);
                } else {
                    sprintf(this->user->bufferSend, "%s", answer);
                    send(this->user->sockfd, this->user->bufferSend, sizeof(this->user->bufferSend), 0);
                }

            } else {
                this->answerClient(300);
                this->offline();
                break;
            }
        }
    }
}

void Session::offline(){
    int ret = recv(this->user->sockfd, this->user->bufferRecv, sizeof(this->user->bufferRecv), 0);
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


int Session::writeFile(){
    char filename[32];
    sprintf(filename, "../offline/%s.txt", reader->username);
    if(getFileSize(filename) < 4136){
        FILE* file = fopen(filename, "a+");
        fprintf(file, "%s: %s\n", this->user->username, this->user->bufferRecv);
        fclose(file);
    } else{
        throw "file is FULL";
    }
    return getFileSize(filename);
}


Session::Session() = default;
Session::Session(int sockfd){
    this->user->sockfd = sockfd;
}

Session::Session(Session& a) = delete;
Session::~Session() = default;