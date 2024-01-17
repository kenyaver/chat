#include "../libs/check.h"
#include "../libs/function.h"
#include "../libs/struct.h"
// #include "../libs/talk.h"

int exitCheck(char* buffer){
    int flag = 0;
    for(int i = 0; i < strlen(buffer); i++){
        if(buffer[i] == '~'){
            flag = -1;
        }
    }
    return flag;
}

char* isClientMessage(char* bufferSRC, char* messageID, char* bufferDST){
    int id;
    parse(bufferSRC, messageID, bufferDST);
    id = fromString(messageID);
    if(id > 0){
        return messageID;
    }
    strcpy(messageID, "-1");
    return messageID;
}

int main(int argc, char* argv[]){
    int sock = socketCheck(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr = initAddrClient(fromString(argv[2]), argv[1]);
    connectCheck(sock, (struct sockaddr*)&addr, sizeof(addr));

    char nameBuffer[20];
    strcat(nameBuffer, argv[3]);
    strcat(nameBuffer, " "); // send your name and partner`s name
    strcat(nameBuffer, argv[4]);
    send(sock, nameBuffer, 20, 0);

    char bufferRecv[1036];
    char bufferSend[1025];
    char messageID[16];

    std::thread reciv([&]{
        while(exitCheck(bufferRecv) == 0 && exitCheck(bufferSend) == 0){
            char buffer[1024];
            int ret = recv(sock, bufferRecv, 1036, 0);
            if(ret > 0){
                isClientMessage(bufferRecv, messageID, buffer);
                if(strcmp(messageID, "-1") != 0){
                    send(sock, messageID, 4, 0);
                    std::cout << buffer;
                    bzero(buffer, 1024);
                } else {
                    std::cout << bufferRecv;
                    bzero(bufferRecv, 1036);
                }
            }
        }
    });

    std::thread sende([&]{
        while(exitCheck(bufferSend) == 0 && exitCheck(bufferRecv) == 0){
            std::cin.getline(bufferSend, 1025);
            if(strlen(bufferSend) == 1025){
                std::cout << "incorrect message (>1024 bytes)\n";
                bzero(bufferSend, 1025);
            } else {
                send(sock, bufferSend, 1024, 0);
                bzero(bufferSend, 1025);
            }
        }
    });

    reciv.join();
    sende.join();
    std::cout << "\ngood bye!\n";
}