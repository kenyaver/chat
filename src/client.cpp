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

int main(int argc, char* argv[]){
    int sock = socketCheck(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(fromString(argv[2])); // connection
    inet_ptonCheck(AF_INET, argv[1], &addr.sin_addr);
    connectCheck(sock, (struct sockaddr*)&addr, sizeof(addr));

    char nameBuffer[20];
    strcat(nameBuffer, argv[3]);
    strcat(nameBuffer, " "); // send your name and partner`s name
    strcat(nameBuffer, argv[4]);
    send(sock, nameBuffer, 20, 0);

    char bufferRecv[1024];
    char bufferSend[1024];

    std::thread reciv([&]{
        while(exitCheck(bufferRecv) == 0 && exitCheck(bufferSend) == 0){
            int ret = recv(sock, bufferRecv, 1024, 0);
            if(ret > 0){
                std::cout << bufferRecv;
            }
        }
    });

    std::thread sende([&]{
        while(exitCheck(bufferSend) == 0 && exitCheck(bufferRecv) == 0){
            std::cin.getline(bufferSend, 1024);
            send(sock, bufferSend, 1024, 0);
        }
    });

    reciv.join();
    sende.join();
    std::cout << "\ngood bye!\n";
}