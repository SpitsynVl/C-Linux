//gcc -Werror -Wall -Wextra -Wnarrowing -Wconversion -Wwrite-strings -Wcast-qual -Wundef -Wstrict-prototypes -Wbad-function-cast -Wlogical-op -Wreturn-type -g -O2 -fwhole-program

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

char dtype_char(unsigned d_type) {
    switch(d_type) {
        case DT_BLK: return 'b';
        case DT_CHR: return 'c';
        case DT_DIR: return 'd';
        case DT_FIFO: return 'p';
        case DT_LNK: return 'l';
        case DT_REG: return '-';
        case DT_SOCK: return 's';
    }
    return '?';
}

char mode_char(mode_t st_mode) {
    switch (st_mode & S_IFMT) {
        case S_IFBLK: return 'b'; 
        case S_IFCHR: return 'c';
        case S_IFDIR: return 'd'; 
        case S_IFIFO: return 'p'; 
        case S_IFLNK: return 'l'; 
        case S_IFREG: return '-'; 
        case S_IFSOCK: return 's';	
    }
    return '?';
}

int main(void) {
    int status = 0;

    DIR* cur_dir = opendir(".");
    if(cur_dir == NULL) {
        perror("Can't open directory\n");
        return 1;
    }

    
    while(1) {
        errno = 0;
        struct dirent* entry = readdir(cur_dir);
        if(entry == NULL) {
            break;
        }
        char file_type = dtype_char(entry->d_type);

        //в случае, когда readdir не смог распознать тип файла, пользуемся lstat
        if(file_type == '?') {
            struct stat sb;
            if(lstat(entry->d_name, &sb) == -1) {
                perror("Error in lstat\n");
                status = 2;
                continue;
            }
            file_type = mode_char(sb.st_mode);
        }

        printf("%c %s\n", file_type, entry->d_name);
    }

    //проверяем, вышли ли мы из while из-за достижения конца директории или из-за ошибки в readdir
    if(errno != 0) {
        perror("Error in readdir\n");
        status = 3;
    }

    if(closedir(cur_dir) == -1) {
        perror("Error in closedir\n");
        status = 4;
    }

    return status;
}