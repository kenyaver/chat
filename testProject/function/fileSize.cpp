#include "../../libs/function.h"

int main(){
    int size = getFileSize("test.txt");
    std::cout << "size of file: " << size << std::endl;
    // int file = open("test.txt", O_RDWR);
    FILE* file = fopen("test.txt", "a+");
    size = getFileSize("test.txt");
    std::cout << "size of file: " << size << std::endl;
    char* str = "Hello";
    int len = strlen(str);
    fprintf(file, "%s", str);
    size = getFileSize("test.txt");
    std::cout << "size of file: " << size << std::endl;
    fclose(file);
    size = getFileSize("test.txt");
    std::cout << "size of file: " << size << std::endl;
}