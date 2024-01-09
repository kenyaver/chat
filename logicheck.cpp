#include "libs/function.h"
#include "libs/struct.h"

int main(int argc, char* argv[]){
    std::vector<Client> client;
    Client tmp{"", nullptr, 1};
    client.push_back(tmp);
    auto iter1 = client.end() - 1;
    auto iter2 = client.end();
    std::cout << iter1.base() << ' ' << iter2.base() << '\n';
}
