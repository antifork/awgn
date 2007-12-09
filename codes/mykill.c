#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <syslog.h>
#include <err.h>

#include <setjmp.h>
#include <signal.h>

extern char *__progname;
const int log_facility = LOG_LOCAL6;

static const char usage_str[]=
        "Usage: %s [OPTIONS] target name (argv[0]) or pid\n\n"
        "   -k SIG..         signal to send (def=SIGTERM)\n"
        "   -s sec           sec. to wait before sending a SIGKILL (def=10)\n"
        "   -h               print this help\n";


#define _sig(x) [x] = #x
char *signal_list[]= {
    _sig(SIGHUP    ), _sig(SIGINT    ),
    _sig(SIGQUIT   ), _sig(SIGILL    ),
    _sig(SIGTRAP   ), _sig(SIGABRT   ),
    _sig(SIGIOT    ), _sig(SIGBUS    ),
    _sig(SIGFPE    ), _sig(SIGKILL   ),
    _sig(SIGUSR1   ), _sig(SIGSEGV   ),
    _sig(SIGUSR2   ), _sig(SIGPIPE   ),
    _sig(SIGALRM   ), _sig(SIGTERM   ),
    _sig(SIGSTKFLT ), _sig(SIGCLD    ),
    _sig(SIGCHLD   ), _sig(SIGCONT   ),
    _sig(SIGSTOP   ), _sig(SIGTSTP   ),
    _sig(SIGTTIN   ), _sig(SIGTTOU   ),
    _sig(SIGURG    ), _sig(SIGXCPU   ),
    _sig(SIGXFSZ   ), _sig(SIGVTALRM ),
    _sig(SIGPROF   ), _sig(SIGWINCH  ),
    _sig(SIGPOLL   ), _sig(SIGIO     ),
    _sig(SIGPWR    ), _sig(SIGSYS    ),
    _sig(SIGUNUSED ),
};


/* 
 * given a process/kernel thread name, it returns its pid (the lowest) or -1
 * if not found.
 */

int get_pid(const char *cmd)
{
        int pid[1024];
        char status[256];
        struct dirent **namelist;
        int ret=-1, n;
        int i=0;
        FILE *f;

        n = scandir("/proc", &namelist, 0, 0);
        if (n<0)
                perror("scandir");
        else
                while (n--) {
                        if ( (pid[i]=atoi(namelist[n]->d_name)) )
                                i++;
                        free(namelist[n]);
                }
                free(namelist);

        for(n=0; n<i ; n++) {
                if ( sprintf(status,"/proc/%d/status", pid[n]) < 0 )
                        continue;
                f = fopen(status,"r");
                if ( f == NULL )
                        continue;
                char *line = NULL ; size_t size;
                if ( getline(&line, &size, f) < 0 ) {
                        goto next;
                }
                if ( sscanf(line,"Name: %s", status) != 1 ) {
                        goto next;
                }
                if ( !strcmp(status, cmd) ) {
                        fclose(f);
                        ret = pid[n];
                        break;
                }
                next:
                        fclose(f);
        }

        return ret;
}

/*
 * given a signal name or number, it returns
 * the corresponding integer 
 */

int getsignum(const char *sig) {
        int ret = -1, i;

        ret = atoi(sig);
        if (ret!=0)
                return ret;
        ret=-1;

        for (i=0; i < sizeof(signal_list)/sizeof(signal_list[0]); i++) {
                if (signal_list[i] == NULL)
                        continue;
                if (!strcmp(sig,signal_list[i])) {
                        ret = i;
                break;
                }
        }

        return ret;
}

jmp_buf env;

void sigalarm(int i) 
{
        longjmp(env,1);        
}

int main(int argc, char *argv[])
{
        char status[256];

        int signum = SIGTERM;
        char *sig  = "SIGTERM";
        int sec = 10;
        int pid = 0;
        int i;

        struct stat s;
        char *endptr;

        while( (i=getopt(argc, argv, "s:k:h"))!= EOF)
        switch(i) {
            case 'k': { 
                sig = optarg;
                signum = getsignum(optarg);
                if (signum <0)
                        errx(1,"unknown signal %s", optarg);
                break;
            }
            case 's': {
                 sec = strtol(optarg,&endptr,0);
                 if ( *endptr != '\0')
                     errx(2, "sec: bad integer argument");
                 break;
            }
            case 'h': printf(usage_str,__progname); exit(0);
        }

        argc -= optind;
        argv += optind;
        
        if (argc<1) {
                printf(usage_str,__progname);
                exit(0);
        }

        pid = atoi(argv[0]);
        if (pid<= 0)
            pid = get_pid(argv[0]);

        if (pid<0)
            errx(2,"%s: no process killed", argv[0]);

        /* send signal.. */
        printf("sending signal %s to pid %d\n", sig, pid);

        if ( kill(pid,signum) < 0 )
            err(3,"kill");

                
        sprintf(status,"/proc/%d/status", pid);

        signal(SIGALRM, sigalarm);

        /* send SIGKILL after timeout... */
        if ( setjmp(env) ) {
                printf("sending signal SIGKILL sent!\n");
                if ( kill(pid,9) )
                    err(4,"kill");
                exit(0);
        }

        /* wait for the process to terminate... */
        alarm(sec);

        for( ; stat(status, &s) == 0 ; )
                usleep(100);

        printf("done.\n");
        return 0;
}
