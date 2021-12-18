#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>  
#include <dirent.h> 
#include <string.h> 
#include <fcntl.h>
  
char mode_letter(unsigned mode) { 

    switch (mode & S_IFMT) {
           case S_IFBLK:  return 'b';
           case S_IFCHR:  return 'c';
           case S_IFDIR:  return 'd';
           case S_IFIFO: return 'f';
           case S_IFLNK:  return 'l';
           case S_IFREG:  return 'r';
           case S_IFSOCK: return 's';
           default:       return '?';
    } 
}



char dtype_letter(unsigned dtype) { 

    switch(dtype) {
        case DT_BLK : return 'b'; break;    

        case DT_CHR : return 'c'; break;

        case DT_DIR : return 'd'; break;   

        case DT_FIFO : return 'f'; break; 

        case DT_LNK : return 'l'; break;

        case DT_REG : return 'r'; break;    

        case DT_SOCK : return 's'; break;    

        case DT_UNKNOWN : return '?'; break;

        default : return '-';
    }

} 

void FileInformation(const char* dirpath) {
    DIR* dir_fd = opendir(dirpath);  
    if(dir_fd == NULL) {
        perror("open dir failed");
        return;
    }  
    //считываем файлы из данного каталога  
    struct dirent* entry; 
    while((entry = readdir(dir_fd)) != NULL) {
        struct stat sb; 
        char type = dtype_letter(entry->d_type); 
        if(type == '?') {
            struct stat sb; 
            if(lstat(entry->d_name, &sb) == 0) {
                type = mode_letter(sb.st_mode);
            } 
        }   
        printf("%c %s\n", type, entry->d_name);
    }
    closedir(dir_fd); 
}


int main(int argc, char** argv)  {    

    if(argc == 1) {
        FileInformation("."); 
    } else { 
        for(int i = 1; i < argc; i++) {
            printf("Current directory: %s\n", argv[i]);  
            FileInformation(argv[i]);
        } 
    }

    return 0;
}