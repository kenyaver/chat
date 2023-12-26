#include "../../libs/struct.h"

int main(){
    std::vector<Client> client;
    client.push_back({"hell", nullptr, 0});
    client.push_back({"EGOR", nullptr, 1});
    client.push_back({"TEA", nullptr, 0});
    client.push_back({"NUBER", nullptr, 1});
    for(;;){
        char user[8];
        std::cin >> user;
        if(userCheck(user, client) == 1 || userCheck(user, client) == -1){
            std::cout << "SUCCESS\n";
        } else {
            std::cout << "FAIL\n";
        }
    }
}