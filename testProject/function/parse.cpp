#include "../../libs/function.h"

int main(){
    char recive[20] = "Artem Andrey\n";
    char username[8];
    char partner[8];
    parse(recive, username, partner);
    std::cout << "username: " << username
            << "|\npartner: " << partner;
}