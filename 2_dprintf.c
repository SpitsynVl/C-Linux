#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h>



int main(int argc, char* argv[]){  

    // три аргумента 0 - испольняемый файл  
    // 1 - в какой файл записываем 
    // 2 - что записываем 

    //проверка корректности данных в консоли
    if(argc < 3) { 
        printf("I need file and string\n"); 
        return 1;
    }  
    if(argc > 3) {
        printf("I need only file and string, nothing more\n");
    }

    int flags = O_WRONLY | O_CREAT;   
     
    int fd = open(argv[1], flags, 0622); 

    //проверка корректности открытия файла
    if (fd < 0) { 
        perror("Failed to open file for writing\n"); 
        return 2; 
    } 

    //запись данных в файл и проверка корректности записи
    if(dprintf(fd, "%s\n",  argv[2]) < 0){ 
        perror("Can't write in file\n"); 
        return 3;  
    } 

    //проверка корректности закрытия файла
    if (close(fd) < 0) {
        perror("Can't close file\n"); 
        return 4;  
    }


    return 0;
}