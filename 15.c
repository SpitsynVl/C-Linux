#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

//struct sigaction {
 //             void     (*sa_handler)(int);
 //            	void     (*sa_sigaction)(int, siginfo_t *, void *);
 //             sigset_t   sa_mask;
 //             int        sa_flags;
 //             void     (*sa_restorer)(void);
 //          };

void handler(int sig) {}

int main(void)
{   
    pid_t inf = getpid(); //получение id процесса 
    printf("PID of Procces %d\n", inf);
    struct sigaction sa = {}; 
    sa.sa_handler = handler; 
    sa.flags = SA_RESTART //перезагрузка библиотечных функций системных вызовов
    for (int sig_num = 1; sig_num <= 31; sig_num++)
        if (sigaction(sig_num, &sa, NULL)) {
            perror("sigaction");
            printf("Failed to set action for signal %i:\n", sig_num);
            psignal(sig_num, NULL);
        }

    for(;;){ 
        pause();
    }
    return 0;
}
