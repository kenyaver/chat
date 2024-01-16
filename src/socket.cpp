#include "../libs/check.h"
#include "../libs/function.h"
#include "../libs/struct.h"
#include "../libs/talk.h"



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
        client.back().handleClient();
        char usernames[20];
        int ret = recv(client.back().sockfd, usernames, 20, 0);
        parse(usernames, client.back().login, )
        if(ret > 0){
            std::cout << client.back().login << " accepted\n";
            
        } else {
            std::cout << errno << std::endl;
        }
    }
    close(sock);
    return 0;
}