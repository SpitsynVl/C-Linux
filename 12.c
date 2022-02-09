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
    const char* unknown = "?";
    struct passwd* rpwuid = getpwuid(ruid);
    const char* rpw_name = rpwuid ? rpwuid->pw_name : unknown;
    struct group* rgrid = getgrgid(rgid);
    const char* rgr_name = rgrid ? rgrid->gr_name : unknown;
    printf("UID %d(%s), GID %d(%s)\n", ruid, rpw_name, rgid, rgr_name);

    //Effective user and group ID
    uid_t euid = geteuid();
    gid_t egid = getegid();
    struct passwd * epwuid = getpwuid(euid);
    const char* epw_name = epwuid ? epwuid->pw_name : unknown;
    struct group * egrid = getgrgid(egid);
    const char* egr_name = egrid ? egrid->gr_name : unknown;
    printf("EUID %d(%s), EGID %d(%s)\n", euid, epw_name, egid, egr_name);

    //Supplementary groups
    int groups_num = getgroups(0, NULL);
    if(groups_num < 0) {
        perror("Error in getgroups\n");
        return 1;
    }
    gid_t groups_list[groups_num];
    groups_num = getgroups(groups_num, groups_list);
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

    return 0;
}