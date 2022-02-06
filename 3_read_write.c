#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>  
#include <unistd.h> 
#include <string.h>
#include <stdbool.h> 
#include <stdlib.h>
#define BUFF_SIZE 1024*1024


int make_copy(int fd_rd, int fd_wr) {
    char buffer[BUFF_SIZE];
    

    ssize_t bytes_read, bytes_write;
    while(true) {
        bytes_read = read(fd_rd, buffer, BUFF_SIZE);

        //ошибка
        if(bytes_read < 0) {
            return 1;
        }

        //конец файла
        if(bytes_read == 0) {
            return 0;
        }
        
        bytes_write = write(fd_wr, buffer, bytes_read);
        if(bytes_write < 0) {
            return 2;
        }
    }
}

int main(int argc, char* argv[]) {
    //проверка корректности данных, введённых в терминале
    if(argc < 3) {
        perror("I need file to read and file to write\n");
        return 1;
    }
    if(argc > 3) {
        perror("I need only file to read and file to write\n");
        return 1;
    }

    if(strcmp(argv[1], argv[2]) == 0) {
        perror("You try to copy file to itself, it's incorrect\n");
        return 2;
    }

    //открытие и проверка корректности открытия файла для чтения
    int fd_read = open(argv[1], O_RDONLY, 0644);
    if(fd_read < 0) {
        perror("Can't open file to read from it\n");
        return 3;
    }

    //открытие и проверка корректности открытия файла для записи
    int fd_write = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(fd_write < 0) {
        close(fd_read);
        perror("Can't open file to write to it\n");
        return 4;
    }

    int status = make_copy(fd_read, fd_write);
    switch(status) {
        case 1:
            perror("Can't read from first file\n");
            break;
        case 2:
            perror("Can't write to second file\n");
            break;
        break;
    }

    if(close(fd_read) < 0) {
        perror("Can't close first file\n");
        return 5;
    }

    if(close(fd_write) < 0) {
        perror("Can't close second file\n");
        return 6;
    }

    return 0;
}
