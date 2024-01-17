#include "../libs/check.h"
#include "../libs/function.h"
#include "../libs/class.h"

int main(){
    int sock = socketCheck(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1500);
    addr.sin_addr.s_addr = INADDR_ANY;
    socklen_t addrLen = sizeof(addr);
    bindCheck(sock, (sockaddr*)&addr, addrLen);
    listenCheck(sock, 20);

    for(;;){
        client.push_back({});
        client.back().sockfd = acceptCheck(sock, (sockaddr*)&addr, &addrLen);
        if(client.back().sockfd == -1){
            continue;
        }
        std::cout << "count if clients: " << client.size() << std::endl;
        client.back().handleClient();
    }
    close(sock);
    return 0;
}