#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>


int main(void) {

    //PID PPID PGID SID
    pid_t pid = getpid();
    pid_t ppid = getppid();
    pid_t pgid = getpgrp();
    pid_t sid = getsid(0);
    printf("PID %d, PPID %d, PGID %d, SID %d\n", pid, ppid, pgid, sid);

    //Real user and group ID
    uid_t ruid = getuid();
    gid_t rgid = getgid();
    char* unknown = malloc(sizeof("?"));
    strcpy(unknown, "?");
    struct passwd* rpwuid = getpwuid(ruid);
    char* rpw_name = rpwuid ? rpwuid->pw_name : unknown;
    struct group* rgrid = getgrgid(rgid);
    char* rgr_name = rgrid ? rgrid->gr_name : unknown;
    printf("UID %d(%s), GID %d(%s)\n", ruid, rpw_name, rgid, rgr_name);

    //Effective user and group ID
    uid_t euid = geteuid();
    gid_t egid = getegid();
    struct passwd * epwuid = getpwuid(euid);
    char * epw_name = epwuid ? epwuid->pw_name : unknown;
    struct group * egrid = getgrgid(egid);
    char * egr_name = egrid ? egrid->gr_name : unknown;
    printf("EUID %d(%s), EGID %d(%s)\n", euid, epw_name, egid, egr_name);

    //Supplementary groups
    gid_t groups_list[NGROUPS_MAX + 1];
    int groups_num = getgroups(NGROUPS_MAX, groups_list);
    if(groups_num < 0) {
        perror("Error in getgroups\n");
        return 1;
    }
    printf("Supplemetary groups(%d): ", groups_num);
    for(int i = 0; i < groups_num; ++i) {
        printf("%d", groups_list[i]);
        struct group* group_info = getgrgid(groups_list[i]);
        if(group_info == NULL) {
            printf("(?) "); 
        }
        else {
            printf("(%s) ", group_info->gr_name);
        }
    }
    printf("\n");

    free(unknown);

    return 0;
}