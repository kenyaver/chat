#include<iostream>
#include<vector>

int main(){
    std::vector<int> a;
    for(int i = 0; i < 5; i++){
        a.push_back(i);
        a[i] *= a[i]; 
        std::cout << a.size();
    }

    for(auto i: a){
        std::cout << i << '\n';
    }
}