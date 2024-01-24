#include "../libs/class.h"

int main(int argc, char* argv[]){
    char IPaddr[100];
    getIPaddr(IPaddr);
    std::cout << "server`s IP-address: " << IPaddr << std::endl;

    int sock;
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(fromString(argv[1]));
    addr.sin_addr.s_addr = INADDR_ANY;
    try{
        sock = createTCPsocket(0);
        bindCheck(sock, reinterpret_cast<sockaddr*>(&addr));
        listenCheck(sock, 20);
    } catch(const char* errorMessage){
        std::cout << errorMessage << std::endl;
        exit(EXIT_FAILURE);
    }

    for(;;){
        try{
            client.push_back({});
            client.back().acceptClient(sock, addr);
            client.back().handleClient();
        } catch(const char* errorMessage){
            std::cout << errorMessage << std::endl;
            continue;
        }
    }
    close(sock);
    return 0;
}