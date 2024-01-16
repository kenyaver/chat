#include "talk.h"

void talk(Client& sender, Client& recver){
struct timespec timeout;
    timeout.tv_sec = 10;
    struct pollfd fidesc;
    fidesc.fd = sock;
    fidesc.events = POLLIN;
    char bufferRecv[1024];
    char bufferSend[1024];
    int afk = 0;
    while(afk < 300000 && !exitClient(bufferRecv) && !exitClient(bufferSend))
}