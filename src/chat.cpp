#include "../libs/check.h"
#include "../libs/function.h"
// #include "../libs/struct.h"

struct param{
    int *first;
    int *second;    
};

int exitCheck(char* buffer){
    int flag = 0;
    for(int i = 0; i < strlen(buffer); i++){
        if(buffer[i] == '~'){
            flag = -1;
        }
    }
    return flag;
}

void* handlePair(void* fd){
    struct param *session = (param*)fd;
    int fds[2]{*session->first, *session->second};
    int ret = socketpair(AF_INET, SOCK_STREAM, 0, fds);
    if(ret != 0){
        std::cout << "create pair failed\n";
    } else {
        std::cout << "pair accepted\n";
    }
    char buffer1[1024];
    char buffer2[1024];
    std::thread reciv([&]{
        while(exitCheck(buffer1) == 0){
            recv(fds[0], buffer1, 1024, 0);
            send(fds[1], buffer1, 1024, 0);
        }
        std::cout << "good bye!\n";
    });

    std::thread sende([&]{
        while(exitCheck(buffer2) == 0){
            recv(fds[1], buffer2, 1024, 0);
            send(fds[0], buffer2, 1024, 0);
        }
        std::cout << "good bye!\n";
    });

    reciv.join();
    sende.join();
    return fd;
}

int main(int argc, char* argv[]){
    std::vector<int*> client;
    pthread_t tid[512]; /* идентификатор потока */
    pthread_attr_t attr; /* отрибуты потока */
    pthread_attr_init(&attr);

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
            struct param* thr = (param*)calloc(1, sizeof(param));
            thr->first = client[i];
            thr->second = client[i-1]; 
            pthread_create(&tid[j], &attr, handlePair, &thr);
            j++;
        }
    }
    for(int t = 0; t < j; t++){
        pthread_join(tid[t], 0);
    }
}