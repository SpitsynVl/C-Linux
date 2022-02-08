#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

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

int main(int argc, char* argv[]) {
    int status = 0;

    if(argc != 2) {
        perror("I need 2 arguments\n");
        return 1;
    }

    DIR* dir_fd = opendir(argv[1]);
    if(dir_fd == NULL) {
        perror("Can't open directory\n");
        return 2;
    }

    int fd = dirfd(dir_fd);
    if(fd < 0) {
        perror("Error in dirfd\n");
        return 3;
    }

    struct dirent* entry;
    errno = 0;
    while((entry = readdir(dir_fd)) != NULL) {
        char file_type = dtype_char(entry->d_type);
        if(file_type == '?') {
            struct stat sb;
            if(fstatat(fd, entry->d_name, &sb, AT_SYMLINK_NOFOLLOW) == -1) {
                perror("Error in fstatat\n");
                status = 4;
                continue;
            }
            file_type = mode_char(sb.st_mode);
        }

        printf("%c %s\n", file_type, entry->d_name);
    }

    if(errno != 0) {
        perror("Error in readdir\n");
        status = 5;
    }

    if(closedir(dir_fd) == -1) {
        perror("Can't close directory\n");
        status = 6;
    }

    return status;
}