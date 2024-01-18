#include<iostream>
#include <vector>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    close(sock);
    close(sock);
}
    