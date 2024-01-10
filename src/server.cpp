#include "../libs/check.h"
#include "../libs/function.h"
#include "../libs/struct.h"
#include "../libs/talk.h"

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
    int j = 0;
    char log_user[24];
    char user[8];
    int sockClient[1024];

    listenCheck(sock, 1024);
    
    for(;;){
        sockClient[i] = acceptCheck(sock, (struct sockaddr*)&addr, &addrLen);
        client.push_back({"", &sockClient[i], 1});
        std::cout << &sockClient[i] << ' ' << client[i].sockfd << '\n';
        int ret = recv(*client[i].sockfd, log_user, 20, 0);
        parse(log_user, client[i].login, user);
        if(loginCheck(client[i].login, client, i) == -1){
            send(*client[i].sockfd, "choose other username~\n", 24, 0);
            close(*client[i].sockfd);
            continue;
        } else {
            send(*client[i].sockfd, "u r ready start message\n", 24, 0);
            t.push_back(std::thread([&]{
                handleClient(client, std::ref(client[i]), user);
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
