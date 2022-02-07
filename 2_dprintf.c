#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h>



int main(int argc, char* argv[]){
    int status = 0;  
    //проверка корректности параметров
    if(argc < 3) { 
        printf("I need file and string\n"); 
        return 1;
    }  
    if(argc > 3) {
        printf("I need only file and string, nothing more\n");
    }
        
    //чтение для всех, запись для владельца
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644); 

    //проверка корректности открытия файла
    if (fd < 0) { 
        perror("Failed to open file for writing\n"); 
        return 2; 
    } 

    //запись данных в файл и проверка корректности записи
    if(dprintf(fd, "%s\n",  argv[2]) < 0) { 
        perror("Can't write in file\n");
        status = 3;
    } 

    //проверка корректности закрытия файла
    if (close(fd) < 0) {
        perror("Can't close file\n"); 
        status = 4;  
    }


    return status;
}