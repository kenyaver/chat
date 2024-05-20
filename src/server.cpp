#include "../include/check.h"
#include "../include/session.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <thread>

char* getIPaddr(char* IPaddr){
    const char* google_dns_server = "8.8.8.8";
    int dns_port = 53;
    
    struct sockaddr_in serv;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(google_dns_server);
    serv.sin_port = htons(dns_port);

    int err = connect(sock, (const struct sockaddr*)&serv, sizeof(serv));

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr*)&name, &namelen);

    inet_ntop(AF_INET, &name.sin_addr, IPaddr, 100);
    close(sock);
    return IPaddr;
}

sockaddr_in addrInit(char* port){
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = INADDR_ANY;
    return addr;
}


int main(int argc, char* argv[]){
    char IPaddr[16]{0};
    getIPaddr(IPaddr);
    std::cout << "server`s IP-address: " << IPaddr << std::endl;

    int sock;
    sockaddr_in addr = addrInit(argv[1]);
    socklen_t addrlen = sizeof(addr);

    try{
        sock = createTCPsocket(0);
        bindCheck(sock, reinterpret_cast<sockaddr*>(&addr));
        listenCheck(sock, 20);
    } catch(const char* errorMessage){
        std::cout << errorMessage << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<std::thread> handler;
    
    for(;;){
        int accepter = accept(sock, (struct sockaddr*)&addr, &addrlen);
        Session session(accepter);
        handler.push_back(std::thread(&Session::worker, session));
        handler.back().detach();
    }
    

    close(sock);
    return 0;
}
