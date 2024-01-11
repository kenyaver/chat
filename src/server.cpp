#include "../libs/check.h"
#include "../libs/function.h"
#include "../libs/struct.h"
#include "../libs/talk.h"

class Server{
    private:
    int sd; // socket descriptor
    public:
    sockaddr_in addr;
    Server(int port){
        sd = socket(AF_INET, SOCK_STREAM, 0);
        if(sd == -1){
            throw std::runtime_error("socket Failed");
        }
        addr = initAddrServer(port);
        int ret = bind(sd, (struct sockaddr*)&addr, sizeof(addr));
        if(ret == -1){
            throw std::runtime_error("bind Failed");
        }
    }

    int getSD(){
        return sd;
    }

    sockaddr_in getAddr(){
        return addr;
    }

    ~Server(){
        close(sd);
    }
};

int main(int argc, char* argv[]){
    // int sock = socketCheck(AF_INET, SOCK_STREAM, 0);
    // struct sockaddr_in addr = initAddrServer(fromString(argv[1]));
    // bindCheck(sock, (struct sockaddr*)&addr, sizeof(addr));
    try{
        Server server(fromString(argv[1]));
        char IPaddr[100];
        getIPaddr(IPaddr);
        std::cout << IPaddr << std::endl;

        socklen_t addrLen = sizeof(server.getAddr());
        std::vector<Client> client;
        int clientSize = 0;
        std::vector<std::thread> t;

        listenCheck(server.getSD(), 20);
        for(;;){
            client.push_back({"", 0, 1});
            client.end()->sockfd = acceptCheck(server.getSD(), (sockaddr*)&server.addr, &addrLen); // accepting
            std::cout << "new client accepted\n";

            t.push_back(std::thread([&]{
                handleClient(client, client[clientSize]); // handle new client
            }));    
            std::cout << "new client handled\n";
            clientSize++;
        }

        for(auto &thr: t){        
            thr.join();
        }
    } catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    

    printf("good bye!\n");
    return 0;
}
