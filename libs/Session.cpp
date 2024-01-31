#include "Session.h"
#include "Client.h"

void Session::talking(){
    char state[12];
    int sessionRes = this->stateSession(state);
    send(this->user->sockfd, state, sizeof(state), 0);
    this->sendOffline();
    if(sessionRes == 1){
        this->talk();
    } else{
        int i = 0;
        while(i < 4 && getFileSize(this->reader->username) < 4128 && this->stateSession(state) == 0){
            this->recverOffline();
        }
        if(i == 4 || getFileSize(this->reader->username) >= 4128){
            send(this->user->sockfd, "limit send to offline client~\n", 32, 0);
            return;
        }
    }
    this->talk();
}


void Session::findReader() noexcept{
    for(auto i: userDB){
        if(i == *this->reader){
            *reader = i;
        }
    }
}

int Session::stateSession(char* state) noexcept{
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

void Session::forwarding(){
    recv(this->user->sockfd, this->user->bufferRecv, sizeof(this->user->bufferRecv), 0);
    this->checkReader(); 
    for(int i = 0; i < 4; i++){
        if(bufferUnconfirm[i] == NULL){
            sprintf(this->bufferUnconfirm[i], "%s", this->user->bufferRecv);          // TODO: сделать проверку на зполненность буффера
            break;
        }
    }
    sprintf(this->user->bufferSend, "%s", this->user->bufferRecv);
    send(this->reader->sockfd, this->user->bufferSend, sizeof(this->user->bufferSend), 0);
}



void Session::clearMessageFromBufferUnconfirm(char* message){
    char* find;
    for(int i = 0; i < 4; i++){
        find = strstr(this->bufferUnconfirm[i], message);
        
        if (find!=NULL) {
            char* find_ = find + strlen(message);
            strcpy (find, find_);
        }
    }
}



void Session::checkReader(){
    char *delim = (char*)" ";
    char *delimFlag = (char*)":";
    char *newReaderFlag = strtok(this->user->bufferRecv, delim);
    if(strstr(newReaderFlag, (char*)":") != NULL){
        char* newReader = strtok(newReaderFlag, delimFlag);
        strcpy(this->reader->username, newReader);
        this->findReader();
    }
}



void Session::recverOffline(){
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

void Session::handleClient(){
    this->reader = new User();
    this->user->status = 1;
    try{
        // this->recvUsernames();
        userDB.push_back(User(this->reader->username, 0));
    } catch(const char* errorMessage){
        std::cout << errorMessage << ": " << errno << '\n';
        throw "client disconnected";
        memset(this, 0, sizeof(this));
        this->user->closeSocket();
        return;
    }

    this->findReader();

    this->talking();


    this->reader->clearUser(this->reader->username);
    delete reader;
    this->user->closeSocket();
}



void Session::talk(){
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
                this->recverOffline();
                break;
            }
        }
    }
}

// bool Session::operator==(Reader& a){
//     return !strcmp(this->username, a.username);
// }

Session::Session() = default;

Session::Session(Session& a) = delete;
Session::~Session() = default;