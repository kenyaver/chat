#include "../libs/check.h"
#include "../libs/function.h"
#include "../libs/struct.h"
#include "../libs/talk.h"

void sendMessage(Client& client, char* recver){
    std::cout << "hh\n";
    send(*client.sockfd, "start message\n", 16, 0);
    struct timespec timeout_spec;
    timeout_spec.tv_sec = 5;
    struct pollfd fidesc;
    fidesc.fd = *client.sockfd;
    fidesc.events = POLLIN;
    int afk = 0;
    int id = 0;
    char buffer[1028];
    int size;
    std::cout << "hmmm..\n";
    while(afk != 300000 && exitClient(buffer) == 0){
        int ret = ppoll(&fidesc, 1, &timeout_spec, NULL);
        if(ret == 0){
            afk += 5000;
            if(afk == 180000){
                send(*client.sockfd, "you innactive!\n", 16, 0);
            }
        }
        if(fidesc.revents && POLLIN){
            fidesc.revents = 0;
            get(client, buffer, id);
            if(size < 4128){
                size = writeFile(client.login, recver, buffer);
            }
        }
    }
    close(*client.sockfd);
}

void readMessage(Client& client){
    char* nameFile;
    strcat(nameFile, "../message/");
    strcat(nameFile, client.login);
    strcat(nameFile, ".txt");
    FILE* file = fopen(nameFile, "a+");
    char buffer[4132];
    while(feof(file)){
        fscanf(file, "%s", buffer);
        send(*client.sockfd, buffer, 4128, 0);
    }
    freopen(nameFile, "w", file);
    fclose(file);
}


int main(int argc, char* argv[]){
    int sock = socketCheck(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = initAddrServer(AF_INET, fromString(argv[1]));
    int bind = bindCheck(sock, (struct sockaddr*)&addr, sizeof(addr));

    char IPaddr[100];
    getIPaddr(IPaddr);
    std::cout << IPaddr << std::endl;

    socklen_t addrLen = sizeof(addr);
    std::vector<Client> client;
    std::vector<std::thread> t;
    int i = 0;
    char log_user[20];
    char user[8];
    int sockClient[1024];

    listenCheck(sock, 1024);
    
    for(;;){
        sockClient[i] = acceptCheck(sock, (struct sockaddr*)&addr, &addrLen);
        client.push_back({"", &sockClient[i], 1});
        int ret = recv(*client[i].sockfd, log_user, 20, 0);
        parse(log_user, client[i].login, user);
        if(loginCheck(client[i].login, client, i) == -1){
            send(*client[i].sockfd, "choose other username~\n", 24, 0);
            close(*client[i].sockfd);
            continue;
        } else {
            int stop = 0;
            t.push_back(std::thread([&]{
                while(stop == 0){
                    readMessage(std::ref(client[i]));
                }
            }));

            t.push_back(std::thread([&]{
                sendMessage(std::ref(client[i]), user);
                stop = 1;
            }));
        }
        std::cout << "iteration OK\n";
        i++;
    }

    for(auto &thr: t){        
        thr.join();
    }
    printf("good bye!\n");
    return 0;
}