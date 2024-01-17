#include "../libs/check.h"
#include "../libs/function.h"
#include "class.h"

// void parse(char* src, char* user1, char* user2);

int main(){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1500);
    addr.sin_addr.s_addr = INADDR_ANY;
    socklen_t addrLen = sizeof(addr);
    bind(sock, (sockaddr*)&addr, addrLen);
    listen(sock, 20);

    for(;;){
        client.push_back({});
        client.back().sockfd = accept(sock, (sockaddr*)&addr, &addrLen);
        std::cout << "count if clients: " << client.size() << std::endl;
        client.back().handleClient();
    }
    close(sock);
    return 0;
}

// void parse(char* src, char* user1, char* user2){
//     int flag = 0;
//     int j = 0;
//     int k = 0;
//     for(int i = 0; i < strlen(src); i++){
//         if(src[i] == ' '){
//             flag++;
//             continue;
//         }
//         if(flag == 0){
//             user1[j] = src[i];
//             j++;
//         } else {
//             user2[k] = src[i];
//             k++;
//         }
//     }
// }