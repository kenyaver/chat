#include "../../libs/check.h"

int main(){
    int sock = socketCheck(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1550);
    inet_pton(AF_INET, (char*)"127.0.0.1", &addr.sin_addr);
    connectCheck(sock, (struct sockaddr*)&addr, sizeof(addr));
    char buffer[32];
    for(;;){
        int read = recv(sock, buffer, 32, 0);
        if(read > 0){
            printf("%s", buffer);
        } else {
            break;
        }
        send(sock, "Hello\n", 1025, 0);
    }
    close(sock);
    printf("%d\n", sock);
}