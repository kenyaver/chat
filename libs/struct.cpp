#include "struct.h"

Client::Client(){
    bzero(login, 8);
    bzero(buffer, 1024);
}

Client::Client(int sockfd){
    this->sockfd = sockfd;
}

Client::Client(const Client& a): Client{a.login}{
    std::cout << this->login << " copied by move-constructor\n";
}

Client::~Client(){
    status = 0;        
    char filename[32];
    sprintf(filename, "../offline%s.txt", login);
    FILE* file = fopen(filename, "a+");
    fprintf(file, "%s", bufferSend);
    fclose(file);
    close(sockfd);
}

void Client::handleClient(std::vector<Client>& client){
    thr = std::thread([&]{
        char usernames[20];
        char recver[8];
        int ret = recv(sockfd, usernames, 20, 0);
        parse(usernames, login, recver);

        send(sockfd, "search partner...\n", 1036, 0);
        char state[8];
        int size = 0;

        Client tmp = findUser(client, recver);
        
        client.push_back(tmp);

        if(client.back().status == 0){
            memcpy(state, "offline\n", 8);
            send(sockfd, state, 1036, 0);
        }

        while(client.back().status == 0){
            client.back().status = userCheckStatus(client, recver);
            if(size < 4128){
                recv(sockfd, buffer, 1024, 0);
                size = writeFile(login, recver, buffer);
            }
        }

        memcpy(state, "online\n", 8);
        send(sockfd, state, 1036, 0);

        int idUser = findIDuser(client, recver);

        std::thread t(talk, std::ref(*this), std::ref(client.at(idUser)));
        std::cout << "Client " << client->login << " started message with " << clients.at(idUser)->login << '\n';
        t.join();
    });

        thr.detach();
        while(status == 1){}
        char bye[16];
        strcpy(bye, "bye!\n");
        send(sockfd, bye, 16, 0);
        std::cout << "end session for client " << login << '\n';
}

char* getIPaddr(char* IPaddr){
    const char* google_dns_server = "8.8.8.8";
    int dns_port = 53;

    struct sockaddr_in serv;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    memset( &serv, 0, sizeof(serv) );
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr( google_dns_server );
    serv.sin_port = htons( dns_port );

    int err = connect( sock , (const struct sockaddr*) &serv , sizeof(serv) );

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr*) &name, &namelen);

    const char* p = inet_ntop(AF_INET, &name.sin_addr, IPaddr, 100);
    close(sock);
    return IPaddr;
}

void inet_ptonCheck(int af, char* src, void* dst){
    int res = inet_pton(af, src, dst);
    if(res == 0){
        printf("fail address\n");
        exit(EXIT_FAILURE);
    }
    if(res == -1){
        perror("fail AF\n");
        exit(EXIT_FAILURE);
    }
}

struct sockaddr_in initAddrServer(int port){
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    return addr;
}

struct sockaddr_in initAddrClient(int port, char* address){
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_ptonCheck(AF_INET, address, &addr.sin_addr);
    return addr;
}

Client findUser(std::vector<Client*>& client, char* user){
    for(int i = 0; i < client.size(); i++){
        if(strcmp(client.at(i)->login, user) == 0){
            return *client.at(i);
        }
    }
    Client tmp(user, 0, 0);
    return tmp;
}

int findIDuser(std::vector<Client*>& client, char* login){
    for(int i = 0; i < client.size(); i++){
        if(strcmp(client.at(i)->login, login) == 0){
            return i;
        }
    }
    return -1;
}

int userCheckStatus(std::vector<Client*>& client, char* user){
    for(int i = 0; i < client.size(); i++){
        if(strcmp(client.at(i)->login, user) == 0){
            return client.at(i)->status;
        }
    }
    return -1;
}