#include "../../libs/function.h"

int main(){
    int x = 10;
    char str[4];
    toString(str, x);
    std::cout << "str: " << str << std::endl;
}