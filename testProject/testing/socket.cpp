#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <vector>

class Client{
    public:
    char login[8];
    int sockfd;
    char buffer[1024];
    std::thread thr;

    Client(){
        bzero(login, 8);
        bzero(buffer, 1024);
    }

    Client(int sockfd){
        this->sockfd = sockfd;
        std::cout << "Client connected\n";
    }

    Client(const Client& a): Client(a.sockfd){
        std::cout << "moving\n";
    }
    
    ~Client(){
        char bye[16];
        strcpy(bye, "bye!\n");
        send(sockfd, bye, 16, 0);
        close(sockfd);
        std::cout << login << " disconnected\n";
        // thr.join();
    }

    void handleClient(){
        thr = std::thread([&]{
        sprintf(buffer, "u cool %s!\n", this->login);
        int err = send(sockfd, buffer, 1024, 0);
        std::cout << err << '\n';
        });

        thr.join();
        this->~Client();
        std::cout << "end session for client\n";
    }
};

int main(){
    std::vector<Client> client;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1501);
    addr.sin_addr.s_addr = INADDR_ANY;
    socklen_t addrLen = sizeof(addr);
    bind(sock, (sockaddr*)&addr, addrLen);
    listen(sock, 20);
    std::vector<int> accepter;
    for(;;){
        Client* zero = new Client();
        zero->sockfd = accept(sock, (sockaddr*)&addr, &addrLen);
        client.push_back(*zero);
        recv(client.back().sockfd, client.back().login, 8, 0);
        std::cout << client.back().login << " accepted\n";
        client.back().handleClient();
        delete zero;
    }
    close(sock);
    return 0;
}