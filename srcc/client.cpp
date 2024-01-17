#include "../libs/check.h"
#include "../libs/function.h"

#define BUFFERrSIZE 1024
#define BUFFERsSize 1025

int main(int argc, char* argv[]){
    if(strlen(argv[1]) < 9 && strlen(argv[2]) < 9){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(1500);
        inet_pton(AF_INET, "192.168.10.92", &addr.sin_addr);
        socklen_t addrLen = sizeof(addr);
        int err = connect(sock, (sockaddr*)&addr, addrLen);
        char usernames[20];
        sprintf(usernames, "%s %s", argv[1], argv[2]);
        send(sock, usernames, 20, 0);
        char bufferR[BUFFERrSIZE];
        char bufferS[BUFFERsSize];
        std::thread r([&]{
            while(exitClient(bufferR) == 0 && exitClient(bufferS) == 0){
                int ret = recv(sock, buffer, BUFFERrSIZE, 0);
                if(ret > 0){
                    std::cout << buffer;
                } else {
                    std::cout << errno << std::endl;
                    break;
                }
            }
        });

        std::thread s([&]{
            while(exitClient(bufferR) == 0 && exitClient(bufferS) == 0){
                std::cin.getline(buffer, BUFFERsSize);
                if(strlen(buffer) == BUFFERsSize){
                    std::cout << "error message\n";
                    break;
                } else {
                    send(sock, buffer, 1024, 0);
                }
            }
        });

        r.join();
        s.join();
        close(sock);
    }
    return 0;
}