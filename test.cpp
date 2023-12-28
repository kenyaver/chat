#include <iostream>
#include <vector>

class Object{
    public:
        int x;
        int y;
};

int main(){
    std::vector<int> vec;
    for(int i = 0; i < 5; i++){
        vec[i] = i * i;
    }

    for(auto i: vec){
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}