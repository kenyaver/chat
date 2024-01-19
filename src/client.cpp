#include "../libs/check.h"
#include "../libs/function.h"

#define BUFFERrSIZE 1024
#define BUFFERsSize 1025

int main(int argc, char* argv[]){
    if(strlen(argv[1]) < 9 && strlen(argv[2]) < 9){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(fromString(argv[1]));
        inet_pton(AF_INET, "192.168.10.92", &addr.sin_addr);
        socklen_t addrLen = sizeof(addr);
        int err = connect(sock, (sockaddr*)&addr, addrLen);
        while(err == -1){
            err = connect(sock, (sockaddr*)&addr, addrLen);
        }
        std::cout << "connected to server\n";
        char usernames[20];
        sprintf(usernames, "%s %s", argv[2], argv[3]);
        err = send(sock, usernames, 20, 0);
        std::cout << "usernames: " << err << " bytes\n";
        char bufferR[BUFFERrSIZE];
        char bufferS[BUFFERsSize];
        char hello[32];
        err = recv(sock, hello, sizeof(hello), 0);
        std::cout << hello << " -> " << err << " bytes\n";
        if(err == -1){
            std::cout << errno << '\n';
            close(sock);
            exit(1);
        }
        char state[12];
        err = recv(sock, state, sizeof(state), 0);
        std:: cout << state << " -> " << err << " bytes\n";
        if(err == -1){
            std::cout << errno << '\n';
            close(sock);
            exit(1);
        }
        std::thread r([&]{
            while(exitClient(bufferR) == 0 && exitClient(bufferS) == 0){
                int ret = recv(sock, bufferR, BUFFERrSIZE, 0);
                if(ret == 0){
                    continue;
                }
                if(ret > 0){
                    std::cout << bufferR << std::endl;
                }
                if(ret == -1){
                    std::cout << errno << '\n';
                }
            }
        });

        std::thread s([&]{
            while(exitClient(bufferR) == 0 && exitClient(bufferS) == 0){
                std::cin.getline(bufferS, BUFFERsSize);
                if(strlen(bufferS) == BUFFERsSize){
                    std::cout << "error message\n";
                    break;
                } else {
                    send(sock, bufferS, 1024, 0);
                }
            }
        });

        r.join();
        s.join();
        close(sock);
    } else {
        std::cout << "error names\n";
    }
    return 0;
}