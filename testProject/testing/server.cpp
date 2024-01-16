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
        // std::cout << "Client connected\n";
    }

    Client(const Client& a): Client(a.sockfd){
        // std::cout << "copied\n";
    }
    
    ~Client(){
        close(sockfd);
    }

    void handleClient(){
        thr = std::thread([&]{
        sprintf(buffer, "u cool %s!\n", this->login);
        int err = send(sockfd, buffer, 1024, 0);
        std::cout << err << '\n';
        });

        thr.join();
        char bye[16];
        strcpy(bye, "bye!\n");
        send(sockfd, bye, 16, 0);
        std::cout << "end session for client\n";
    }
};

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
        char usernames[20];
        int ret = recv(client.back().sockfd, usernames, 20, 0);
        if(ret > 0){
            std::cout << client.back().login << " accepted\n";
            client.back().handleClient();
        } else {
            std::cout << ret << ": instability\n";
            std::cout << errno << std::endl;
        }
    }
    close(sock);
    return 0;
}