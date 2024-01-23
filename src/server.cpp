#include "../libs/check.h"
#include "../libs/function.h"
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
    socklen_t addrLen = sizeof(addr);
    try{
        sock = socketCheck(AF_INET, SOCK_STREAM, 0);
        bindCheck(sock, (sockaddr*)&addr, addrLen);
        listenCheck(sock, 20);
    } catch(const char* errorMessage){
        std::cout << errorMessage << std::endl;
        exit(EXIT_FAILURE);
    }

    for(;;){
        try{
            // client.push_back(Client(sock, (sockaddr*)&addr, &addrLen)); // добавляет в вектор класс, ожидающий подключения клиента
            client.push_back({});
            client.back().acceptClient(sock, addr);
            client.back().handleClient();
        } catch(const char* errorMessage){
            std::cout << errorMessage << std::endl;
            // exit(EXIT_FAILURE);
            continue;
        }
        // t.push_back(std::thread(&Client::handleClient, std::ref(client.back())));
        // t.back().detach();
    }
    close(sock);
    return 0;
}