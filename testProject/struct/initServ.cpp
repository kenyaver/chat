#include "../../libs/struct.h"

int main(){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr = initAddrServer(AF_INET, 1550);
    if(addr.sin_family == AF_INET && addr.sin_port == htons(1550) && addr.sin_addr.s_addr == INADDR_ANY){
        std::cout << "SUCCESS\n";
    } else {
        std::cout << "FAIL\n";
    }
}