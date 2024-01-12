#include "../libs/check.h"
#include "../libs/function.h"
#include "../libs/struct.h"
#include "../libs/talk.h"

class Server{
    public:
    int sd;
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

    ~Server(){
        close(sd);
    }
};

int main(int argc, char* argv[]){
    try{
        Server server(fromString(argv[1]));
        char IPaddr[100];
        getIPaddr(IPaddr);
        std::cout << IPaddr << std::endl;

        socklen_t addrLen = sizeof(server.addr);
        std::vector<Client*> client;
        int clientCount = 0;
        std::vector<std::thread> t;
        Client* zero = new Client();
        listenCheck(server.sd, 20);
        for(;;){
            client.push_back(zero);
            client.at(clientCount)->sockfd = acceptCheck(server.sd, (sockaddr*)&server.addr, &addrLen); // accepting
            std::cout << "new client accepted\n";

            t.push_back(std::thread([&]{
                handleClient(client, client.at(clientCount)); // handle new client
            }));    
            std::cout << "new client handled\n";
            clientCount++;
        }

        for(auto &thr: t){        
            thr.join();
        }
        delete zero;
    } catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    

    printf("good bye!\n");
    return 0;
}
