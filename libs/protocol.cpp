#include "protocol.h"

void Protocol::setMessageStatus(){
    sprintf(message, "%d: %d", messageID, statusCode);
}

void Protocol::recvUsernames(){
    char usernames[20];
    int ret = recv(this->user->sockfd, usernames, sizeof(usernames), 0);
    if(ret > 0){
        parseNames(usernames, this->user->username, reader->username);
        std::cout << this->user->username << " connected\n";
    } else {
        throw "error recv usernames";
    }
}

void Protocol::answerClient(int statusCode){
    sprintf(this->user->bufferSend, "%d: %d", messageID, statusCode);
    send(this->user->sockfd, this->user->bufferSend, sizeof(this->user->bufferSend), 0);
}

int Protocol::answerCheck(char* answer){
    int answerInt = fromString(answer);
    if(answerInt != -1){
        if(answerInt == this->messageID){
            return 0;
        }
    }
    return 1;
}

void Protocol::findReader() noexcept{
    for(auto i: userDB){
        if(i == *this->reader){
            *reader = i;
        }
    }
}

int Protocol::stateSession(char* state) noexcept{
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

void Protocol::sendOffline(){
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

void Protocol::forwarding(){
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

void Protocol::clearMessageFromBufferUnconfirm(char* message){
    char* find;
    for(int i = 0; i < 4; i++){
        find = strstr(this->bufferUnconfirm[i], message);
        
        if (find!=NULL) {
            char* find_ = find + strlen(message);
            strcpy (find, find_);
        }
    }
}

void Protocol::checkReader(){
    char *delim = (char*)" ";
    char *delimFlag = (char*)":";
    char *newReaderFlag = strtok(this->user->bufferRecv, delim);
    if(strstr(newReaderFlag, (char*)":") != NULL){
        char* newReader = strtok(newReaderFlag, delimFlag);
        strcpy(this->reader->username, newReader);
        this->findReader();
    }
}

int Protocol::writeFile(){
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