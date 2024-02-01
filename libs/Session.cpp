#include "Session.h"

void Session::handleClient(){
    this->partner = new User();
    this->user = new User();
    this->user->status = 1;
    try{
        int ret = recv(this->user->sockfd, this->user->bufferRecv, sizeof(this->user->bufferRecv), 0);
        this->protocol.parseUsernames(this->user->bufferRecv, this->user->username, this->partner->username);
        userDB.push_back(User(this->partner->username, 0));
    } catch(const char* errorMessage){
        std::cout << errorMessage << ": " << errno << '\n';
        throw "client disconnected";
        memset(this, 0, sizeof(this));
        this->user->closeSocket();
        return;
    }

    this->protocol.findUser(*partner);

    this->setMod();

    this->user->disconnectUser();

    delete partner;
    delete user;
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
        while(i < 4 && getFileSize(this->partner->username) < 4128 && this->stateSession(state) == 0){
            this->offline();
        }
        if(i == 4 || getFileSize(this->partner->username) >= 4128){
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
            if(keepAlive(this->partner->sockfd) == 0){
                recv(this->partner->sockfd, answer, sizeof(answer), 0);

                if(this->protocol.answerCheck(answer)){
                    this->protocol.answerClient(200);
                    this->protocol.clearMessageFromBufferUnconfirm(answer);
                } else {
                    sprintf(this->user->bufferSend, "%s", answer);
                    send(this->user->sockfd, this->user->bufferSend, sizeof(this->user->bufferSend), 0);
                }

            } else {
                this->protocol.answerClient(300);
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
        this->protocol.writeFile();
    } catch(const char* errorMessage){
        std::cout << errorMessage << std::endl;
        return;
    }
}

void Session::forwarding(){
    recv(this->user->sockfd, this->user->bufferRecv, sizeof(this->user->bufferRecv), 0);
    if(this->protocol.checkUser(this->user->bufferRecv) == 1){
        // смена получателя
    }
    this->messageID = this->protocol.parseMessage(this->user->bufferRecv);
    this->protocol.appBufferUnconfirm(this->messageID, this->user->bufferRecv);
    sprintf(this->user->bufferSend, "%s", this->user->bufferRecv);
    send(this->partner->sockfd, this->user->bufferSend, sizeof(this->user->bufferSend), 0);
}


int Session::stateSession(char* state) noexcept{
    int res;

    this->protocol.findUser(*partner);
    if(this->partner->status == 0){
        strcpy(state, "offline\n");
        res = 0;
    } else {
        strcpy(state, "online\n");
        res = 1;
    }

    return res;
}

void Session::sendOffline(){
    char *filename;
    sprintf(filename, "../offline/%s.txt", this->user->username);
    FILE* file = fopen(filename, "r");
    char offlineMessage[12];
    if(file){
        int c;
        strcpy(offlineMessage, "message");
        send(this->user->sockfd, offlineMessage, sizeof(offlineMessage), 0);
        for(int i = 0; i < 4; i++){
            if((c = fgetc(file)) != EOF){
                fgets(this->user->bufferSend, sizeof(this->user->bufferSend), file);
                send(this->user->sockfd, this->user->bufferSend, sizeof(this->user->bufferSend), 0);
            }
        }
    } else {
        strcpy(offlineMessage, "no message");
        send(this->user->sockfd, offlineMessage, sizeof(offlineMessage), 0);
    }
}


Session::Session() = default;
Session::Session(int sockfd){
    this->user->sockfd = sockfd;
}
Session::Session(Session& a) = default;
Session::~Session() = default;