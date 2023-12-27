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

void handlePair(int* fds[]){
    char buffer1[1024];
    char buffer2[1024];
    std::thread reciv([&]{
        while(exitCheck(buffer1) == 0){
            recv(*fds[0], buffer1, 1024, 0);
            send(*fds[1], buffer1, 1024, 0);
        }
    });

    std::thread sende([&]{
        while(exitCheck(buffer2) == 0){
            recv(*fds[1], buffer2, 1024, 0);
            send(*fds[0], buffer2, 1024, 0);
        }
    });
    std::cout << "good bye!\n";
    reciv.join();
    sende.join();
    close(*fds[0]);
    close(*fds[1]);
    delete[] *fds;
}

int main(int argc, char* argv[]){
    std::vector<int*> client;
    std::vector<std::thread> t;

    int sock = socketCheck(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    socklen_t size = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1551);
    addr.sin_addr.s_addr = INADDR_ANY;
    bindCheck(sock, (struct sockaddr*)&addr, sizeof(addr));
    listen(sock, 1024);
    int i = 0;
    int j = 0;
    for(;;){
        int c = accept(sock, (struct sockaddr*)&addr, &size);
        client.push_back(&c);
        i++;
        std::cout << "client accepted\n";
        if(i % 2 == 0){
            int** fds = new int*[2];
            fds[0] = client[i - 2];
            fds[1] = client[i - 1];
            std::cout << *fds[0] << ' ' << *fds[1] << '\n';
            t.push_back(std::thread(handlePair, fds));
            std::cout << "session " << t[j].get_id() << ": created\n"; 
            j++;
        }
    }
    
    for(auto &i: t){
        i.join();
    }

}