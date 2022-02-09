#include <stdio.h>
#include <sys/vfs.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/statvfs.h>

void print_file_syst_info(struct statvfs* sb) {
    printf("Total: %ju\n", (uintmax_t)sb->f_blocks * sb->f_frsize);
    printf("Available: %ju\n", (uintmax_t)sb->f_bfree * sb->f_frsize);
    printf("Available for unpriveleged users: %ju\n", (uintmax_t)sb->f_bavail * sb->f_frsize);
    printf("Used: %ju\n", (uintmax_t)(sb->f_blocks  - sb->f_bfree) * sb->f_frsize);
}


int main(int argc, char* argv[]) {
    if(argc < 2) { 
        perror("I need paths\n");
        return 1;
    }
    struct statvfs sb;
    for(int i = 1; i < argc; i++) {
        if(statvfs(argv[i], &sb) == -1) {
            perror("Error in statvfs\n");
            continue;
        }
        print_file_syst_info(&sb);
        if(i != argc - 1) {
            printf("\n");
        }
    }

    return 0;
}