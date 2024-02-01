#include "protocol.h"
void Protocol::parseUsernames(char* usernames, char* usernameUser, char* usernamePartner){
        parseNames(usernames, usernameUser, usernamePartner);
        std::cout << usernameUser << " connected\n";
}

void Protocol::findUser(User& client) noexcept{
    for(auto i: userDB){
        if(i == client){
            client = i;
        }
    }
}

void Protocol::setMessageStatus(char* message, int messageID, int statusCode){
    sprintf(message, "%d: %d", messageID, statusCode);
}

int Protocol::parseMessage(char* message){
    char messageID[4];
    int i = 0;
    while(message[i] != ' '){
        messageID[i] = message[i];
        message[i] = ' ';
    }
    return fromString(messageID);
}

void Protocol::appBufferUnconfirm(int messageID, char* message){
    this->unconfirm[messageID] = message;
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

int Protocol::checkUser(char* message){
    char *delim = (char*)" ";
    char *delimFlag = (char*)":";
    char *newReaderFlag = strtok(message, delim);
    if(strstr(newReaderFlag, (char*)":") != NULL){
        return 1;
    } else {
        return 0;
    }
}

int Protocol::writeFile(){
    char filename[32];
    sprintf(filename, "../offline/%s.txt", partner->username);
    if(getFileSize(filename) < 4136){
        FILE* file = fopen(filename, "a+");
        fprintf(file, "%s: %s\n", this->user->username, this->user->bufferRecv);
        fclose(file);
    } else{
        throw "file is FULL";
    }
    return getFileSize(filename);
}