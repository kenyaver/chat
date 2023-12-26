#include "../../libs/function.h"

int main(){
    char recive[1024] = "1 Hello\n";
    char idStr[4];
    char message[1024];
    parse(recive, idStr, message);
    std::cout << "id: " << idStr
            << "\nmessage: " << message;
}