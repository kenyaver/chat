#include "struct.h"

Client::Client(){
    bzero(login, 8);
    sockfd = 0;
    status = 1;
    bzero(bufferRecv, 1024);
    bzero(bufferSend, 1032);
}

Client::Client(const char* login, int sockfd, int status, const char* bufferRecv, const char* bufferSend){
    strcpy(this->login, login);
    this->sockfd = sockfd;
    this->status = status;
    strcpy(this->bufferRecv, bufferRecv);
    strcpy(this->bufferSend, bufferSend);
}

Client::Client(const Client& a): Client{a.login, a.sockfd, a.status, a.bufferRecv, a.bufferSend}{
    std::cout << "Client " << this->login << " copied by move-constructor\n";
}



char* Client::getData(int id){
    recv(sockfd, bufferRecv, 1024, 0);
    char* strId = toString(strId, id);
    id++;
    strcat(bufferSend, strId);
    strcat(bufferSend, " ");
    strcat(bufferSend, bufferRecv);
    return bufferSend;
}

int Client::sendData(Client* recver){
    send(recver->sockfd, bufferSend, 1032, 0);
    if(keepAlive(recver->sockfd) != 0){
        close(recver->sockfd);
        memcpy(recver->login, "", 8);
        recver->status = 0;
        return -1;
    }
    return 0;
}

Client::~Client(){
        std::cout << "client " << login <<" disconnected\n";
        close(sockfd);
        char filename[32];
        strcat(filename, "../offline");
        strcat(filename, login);
        strcat(filename, ".txt");
        FILE* file = fopen(filename, "a+");
        fprintf(file, "%s", bufferSend);
        fclose(file);
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
    Client tmp(user, 0, 0, "", "");
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

int loginCheck(char* log, std::vector<Client*>& client){
    int flag = 0;
    for(int i = 0; i < client.size() - 1; i++){
        if(strcmp(log, client.at(i)->login) == 0){
            if(client.at(i)->status == 1){
                flag = -1;
            }
        }
    }
    if(flag == 0){
        return 0;
    } else {
        return -1;
    }
}

int userCheck(char* user, std::vector<Client>& client){
    for(int i = 0; i < client.size(); i++){
        if(strcmp(client[i].login, user) == 0){
            if(client[i].status == 1){
                return 1;
            } else {
                return -1;
            }
        }
    }
    return 0;
}