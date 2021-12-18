#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <pwd.h>
#include <stdlib.h>
#include <errno.h>
#include <grp.h>  
#include <limits.h> 

int main(void)
{
    pid_t th_pid = getpid();
    pid_t th_ppid = getppid();
    gid_t th_egid = getegid();
    pid_t th_sid = getsid(th_pid);
    printf(
        "PID = [%d], Parent PID = [%d], EGID = [%d], SID = [%d]\n",
        th_pid, th_ppid, th_egid, th_sid
    );

    int priority = getpriority(PRIO_PROCESS, th_pid); //приоритет процесса
    if ((priority == -1) && (errno != 0)) {
        perror("getpriority");
        return 4;
    }
    printf("Scheduling priority: [%d]\n", priority); 

    gid_t groups_list[NGROUPS_MAX + 1]; //общее кол-во идентификаторов дополнительных групп для текущего процесса
    int groups_list_size = NGROUPS_MAX; 
    int numGroups = getgroups(groups_list_size, groups_list);
    if (numGroups < 0) {
        perror("getgroups");
        return 2;
    }

    putchar('\n');
    
    struct passwd *pass = getpwuid(getuid());
    printf(
        "User: '%s'\nPassword: '%s'\nUID: [%i]\nGID: [%i]\n", 
        pass->pw_name, pass->pw_passwd, pass->pw_uid, pass->pw_gid
    );
    return 0;
}