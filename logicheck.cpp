#include "../../libs/function.h"
#include "../../libs/struct.h"

int main(int argc, char* argv[]){
    std::vector<Client> client;
    Client tmp{"", nullptr, 1};
    char user[8];
    char partner[8];
    char nameBuffer[20];
    strcat(nameBuffer, argv[1]);
    strcat(nameBuffer, " ");
    strcat(nameBuffer, argv[2]);
    std::cout << nameBuffer << '\n';
    parse(nameBuffer, user, partner);
    std::cout << user << "|\n"
            << partner << "|\n";
    loginCheck(user, client, 0);
}
