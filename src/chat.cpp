#include "../libs/check.h"
#include "../libs/function.h"
// #include "../libs/struct.h"

int exitCheck(char* buffer){
    int flag = 0;
    for(int i = 0; i < strlen(buffer); i++){
        if(buffer[i] == '~'){
            flag = -1;
        }
    }
    return flag;
}

void handlePair(int* client1, int* client2){
    std::cout << client1 << ' ' << client2 << '\n';
    char buffer1[1024];
    char buffer2[1024];

    std::thread c1([&]{
        strcpy(buffer1, "hello client1\n");
        send(*client1, buffer1, 1024, 0);
        while(exitCheck(buffer1) == 0 && exitCheck(buffer2) == 0){
            int ret = recv(*client1, buffer1, 1024, 0);
            if(ret > 0){
                send(*client2, buffer1, 1024, 0);
            }
        }
    });

    std::thread c2([&]{
        strcpy(buffer2, "Hello client2\n");
        send(*client2, buffer2, 1024, 0);
        while(exitCheck(buffer2) == 0 && exitCheck(buffer1) == 0){
            int ret = recv(*client2, buffer2, 1024, 0);
            if(ret > 0){
                send(*client1, buffer2, 1024, 0);
            }
        }
    });
    c1.join();
    c2.join();
    std::cout << "session " << std::this_thread::get_id() << ": finihed\n";
    close(*client1);
    close(*client2);
}

int main(int argc, char* argv[]){
    std::vector<int*> client;
    std::vector<std::thread> t;

    int sock = socketCheck(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    socklen_t size = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1555);
    addr.sin_addr.s_addr = INADDR_ANY;
    bindCheck(sock, (struct sockaddr*)&addr, sizeof(addr));
    listen(sock, 1024);
    int accep[1024];
    int i = 0;
    int j = 0;
    for(;;){
        accep[i] = accept(sock, (struct sockaddr*)&addr, &size);
        client.push_back(&accep[i]);
        i++;
        std::cout << "client accepted\n";
        if(i % 2 == 0){
            int* client1 = client[i - 2];
            int* client2 = client[i - 1];
            t.push_back(std::thread(handlePair, client1, client2));
            std::cout << "session " << t[j].get_id() << ": created\n"; 
            j++;
        }
    }
    
    for(auto &i: t){
        i.join();
    }

}