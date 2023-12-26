#include "../libs/check.h"
#include "../libs/function.h"
#include "../libs/struct.h"
#include "../libs/talk.h"

int main(int argc, char* argv[]){
    int sock = socketCheck(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = initAddrServer(AF_INET, fromString(argv[1]));
    int bind = bindCheck(sock, (struct sockaddr*)&addr, sizeof(addr));
    socklen_t addrLen = sizeof(addr);
    std::vector<Client> client;
    int i = 0;
    char log_user[24];
    char user[8];
    listenCheck(sock, 1024);
    for(;;){
        int sockClient = acceptCheck(sock, (struct sockaddr*)&addr, &addrLen);
        client.push_back({"", &sockClient, 1});
        recv(*client[i].sockfd, log_user, 24, 0);
        parse(log_user, client[i].login, user);
        if(loginCheck(client[i].login, client, i) != 0){
            send(*client[i].sockfd, "choose other username\n", 24, 0);
            close(*client[i].sockfd);
            continue;
        }
        std::thread handle(handleClient, std::move(client), std::move(client[i]), user);
        handle.join();
        i++;
    }
    printf("good bye!\n");
    return 0;
}
