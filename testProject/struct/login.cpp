#include "../../libs/struct.h"

int main(){
    std::vector<Client> ClientDB;
    int i = 0;
    for(;;){
        ClientDB.push_back({"", nullptr, 0});
        char log[8];
        std::cin >> log;
        if(loginCheck(log, ClientDB, i) == 0){
            std::cout << "SUCCESS\n";
        } else {
            std::cout << "FAIL\n";
            // ClientDB.erase(ClientDB.end());
            memmove(&ClientDB[i], &ClientDB[i+1], (ClientDB.size()-i-1) * sizeof(int));
            i--;
        }
        i++;
    }
}