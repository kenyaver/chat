#include "../libs/check.h"
#include "../libs/function.h"
#include "../libs/class.h"

int main(int argc, char* argv[]){
    char IPaddr[100];
    getIPaddr(IPaddr);
    std::cout << "server`s IP-address: " << IPaddr << std::endl;

    std::vector<std::thread> t;

    int sock = socketCheck(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(fromString(argv[1]));
    addr.sin_addr.s_addr = INADDR_ANY;
    socklen_t addrLen = sizeof(addr);
    bindCheck(sock, (sockaddr*)&addr, addrLen);
    listenCheck(sock, 20);

    for(;;){
        client.push_back({});
        client.back().sockfd = acceptCheck(sock, (sockaddr*)&addr, &addrLen);
        if(client.back().sockfd == -1){
            std::cout << "EBADF\n";
            continue;
        }
        std::cout << "count if clients: " << client.size() << std::endl;
        // t.push_back(std::thread([&]{
        //     client.back().handleClient();
        // }));

        t.push_back(std::thread(&Client::handleClient, client.back()));
        t.back().detach();
    }
    close(sock);
    return 0;
}