#include "../../libs/check.h"

int main(){
    int sock = socketCheck(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1550);
    addr.sin_addr.s_addr = INADDR_ANY;
    socklen_t addrLen = sizeof(addr);
    std::vector<int> client;
    int i = 0;
    char buf[1024];
    int ret = bindCheck(sock, (struct sockaddr*)&addr, addrLen);
    listenCheck(sock, 3);
    for(;;){
        int tmpClient = acceptCheck(sock, (struct sockaddr*)&addr, &addrLen);
        client.push_back(tmpClient);
        printf("client accepted\n");
        send(client[i], "Hello user!\n", 32, 0);
        recv(client[i], buf, 1024, 0);
        if(buffCheck(buf) == -1){
            break;
        } else {
            send(client[i], "OK\n", 4, 0);
        }
        i++;
    }
    for(int i = 0; i < 4; i++){
        close(client[i]);
        printf("%d", client[i]);
    }
    close(sock);
    printf("%d", sock);
}