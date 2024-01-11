#include "../libs/check.h"
#include "../libs/function.h"
#include "../libs/struct.h"
#include "../libs/talk.h"

int main(int argc, char* argv[]){
    int sock = socketCheck(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = initAddrServer(AF_INET, fromString(argv[1]));
    bindCheck(sock, (struct sockaddr*)&addr, sizeof(addr));

    char IPaddr[100];
    getIPaddr(IPaddr);
    std::cout << IPaddr << std::endl;

    socklen_t addrLen = sizeof(addr);
    std::vector<Client> client;
    int clientSize = 0;
    std::vector<std::thread> t;

    listenCheck(sock, 1024);
    
    for(;;){
        client.push_back({"", 0, 1});
        client.end()->sockfd = acceptCheck(sock, (struct sockaddr*)&addr, &addrLen); // accepting
        std::cout << "new client accepted\n";

        t.push_back(std::thread([&]{
            handleClient(client, client[clientSize]); // handle new client
        }));
        std::cout << "new client handled\n";
        clientSize++;
    }

    for(auto &thr: t){        
        thr.join();
    }

    printf("good bye!\n");
    close(sock);
    return 0;
}
