#include "../libs/check.h"
#include "../libs/function.h"

#define BUFFERrSIZE 1024
#define BUFFERsSize 1025

int main(int argc, char* argv[]){
    if(strlen(argv[3]) < 9 && strlen(argv[4]) < 9){
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(fromString(argv[2]));
        inet_pton(AF_INET, argv[1], &addr.sin_addr);
        socklen_t addrLen = sizeof(addr);

        int err = connect(sock, (sockaddr*)&addr, addrLen);
        if(err == -1){
            err = connect(sock, (sockaddr*)&addr, addrLen);
            std::cout << "connect error: " << errno << '\n';
            exit(EXIT_FAILURE);
        }
        std::cout << "connected to server\n";

        char usernames[20];
        sprintf(usernames, "%s %s", argv[3], argv[4]);
        err = send(sock, usernames, sizeof(usernames), 0);
        if(err == -1){
            std::cout << errno << '\n';
            close(sock);
            exit(EXIT_FAILURE);
        }
        std::cout << "usernames: " << err << " bytes\n";
        
        char hello[32];

        err = recv(sock, hello, sizeof(hello), MSG_WAITALL);
        if(err < 1){
            std::cout << errno << '\n';
            close(sock);
            exit(EXIT_FAILURE);
        }
        std::cout << hello << " -> " << err << " bytes\n"; // !!

        char state[12];
        err = recv(sock, state, sizeof(state), MSG_WAITALL);
        if(err < 1){
            std::cout << errno << '\n';
            close(sock);
            exit(EXIT_FAILURE);
        }
        std:: cout << state << " -> " << err << " bytes\n";
        
        char bufferR[BUFFERrSIZE];
        char bufferS[BUFFERsSize];
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