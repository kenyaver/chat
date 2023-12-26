#include "../../libs/struct.h"

int main(){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr = initAddrClient(AF_INET, 1550, (char*)"127.0.0.1");
    if(addr.sin_family == AF_INET && addr.sin_port == htons(1550)){
        std::cout << "SUCCESS\n";
    } else {
        std::cout << "FAIL\n";
    }
}