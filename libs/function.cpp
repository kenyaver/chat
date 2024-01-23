#include "function.h"

char* toString(char* str, int val)
{
    sprintf(str, "%d", val);
    return str;
}

int fromString(const char* s) 
{
  int res = atoi(s);
  if(res == 0){
    res = -1;
  }
  return res;
}

char* getIPaddr(char* IPaddr){
    const char* google_dns_server = "8.8.8.8";
    int dns_port = 53;

    struct sockaddr_in serv;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    memset( &serv, 0, sizeof(serv) );
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr( google_dns_server );
    serv.sin_port = htons( dns_port );

    int err = connect( sock , (const struct sockaddr*) &serv , sizeof(serv) );

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr*) &name, &namelen);

    const char* p = inet_ntop(AF_INET, &name.sin_addr, IPaddr, 100);
    close(sock);
    return IPaddr;
}

int getFileSize(const char* fileName){
	int file_size = 0;
	struct stat fileStatbuff;
	int fd = open(fileName, O_RDONLY);
	if(fd == -1){
		file_size = -1;
	}
	else{
		if ((fstat(fd, &fileStatbuff) != 0) || (!S_ISREG(fileStatbuff.st_mode))) {
			file_size = -1;
		}
		else{
			file_size = fileStatbuff.st_size;
		}
		close(fd);
	}
	return file_size;
}

void parseNames(const char* src, char* dst1, char* dst2){
    char sep[10] = " \0";
    char *isrc;

    isrc = strtok (const_cast<char*>(src), sep);
    strcpy(dst1, isrc);
        
    isrc = strtok (NULL, sep);
    strcpy(dst2, isrc);
}

// void parse(char* src, char* dst1, char* dst2){
//     std::cout << "start parsing\n";
//     int size = strlen(src);
//     int flagdst = 0;
//     int j = 0;
//     int k = 0;
//     for(int i = 0; i < size; i++){
//         if(src[i] == ' '){
//             flagdst++;
//             continue;
//         }
//         if(flagdst == 0){
//             dst1[j] = src[i];
//             j++;
//         } else {
//             if(src[i] != '\0'){
//                 dst2[k] = src[i];
//                 k++;
//             } else {
//                 break;
//             }
//         }
//     }
//     std::cout << dst1 << ' ' << dst2 << '\n';
// }