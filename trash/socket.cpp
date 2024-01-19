#include "../libs/check.h"
#include "../libs/function.h"




int main(){
    std::vector<Client> client;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1500);
    addr.sin_addr.s_addr = INADDR_ANY;
    socklen_t addrLen = sizeof(addr);
    bind(sock, (sockaddr*)&addr, addrLen);
    listen(sock, 20);

    for(;;){
        client.push_back({});
        client.back().sockfd = accept(sock, (sockaddr*)&addr, &addrLen);
        std::cout << "count if clients: " << client.size() << std::endl;
        client.back().handleClient(client);
    }
    close(sock);
    return 0;
}