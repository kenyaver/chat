#include "command.h"

void setNetCommandToHost(Command& h){
    h.header.len = ntohs(h.header.len);
    h.header.type = ntohs(h.header.type);
    h.header.messageID = ntohs(h.header.messageID);
}

int recvCommand(int sockfd, Command* buffer){
    int byte = recv(sockfd, &buffer->header, sizeof(Header), 0);
    if(byte == -1){
        return byte;
    }
    setNetCommandToHost(*buffer);
    buffer = (Command*)realloc(buffer, buffer->header.len);
    byte += recv(sockfd, buffer->message, buffer->header.len - sizeof(Header), 0);
    return byte;
}

void setHostCommandToNet(Command& h){
    h.header.len = htons(h.header.len);
    h.header.type = htons(h.header.type);
    h.header.messageID = htons(h.header.messageID);
}

int sendCommand(int sockfd, Command& buffer){
    setHostCommandToNet(buffer);
    int byte = send(sockfd, &buffer, buffer.header.len, 0);
    return byte;
}
