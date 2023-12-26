#include "struct.h"

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

struct sockaddr_in initAddrServer(int af, int port){
    struct sockaddr_in addr;
    addr.sin_family = af;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    return addr;
}

struct sockaddr_in initAddrClient(int af, int port, char* address){
    struct sockaddr_in addr;
    addr.sin_family = af;
    addr.sin_port = htons(port);
    inet_ptonCheck(af, address, &addr.sin_addr);
    return addr;
}

Client findUser(std::vector<Client>& client, char* user){
    for(int i = 0; i < 16; i++){
        if(strcmp(client[i].login, user) == 0){
            return client[i];
        }
    }
    Client tmp;
    memcpy(tmp.login, user, 8);
    tmp.sockfd = nullptr;
    tmp.status = 0;
    return tmp;
}

int loginCheck(char* log, std::vector<Client>& client, int id){
    int flag = 0;
    for(int i = 0; i < client.size(); i++){
        if(strcmp(log, client[i].login) == 0){
            flag = -1;
        }
    }
    if(flag == 0){
        strcpy(client[id].login, log);
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