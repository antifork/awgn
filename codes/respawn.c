/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>
#include <err.h>

extern char *__progname; 

const int log_facility = LOG_LOCAL6;

static const char usage_str[]=
        "Usage: %s [OPTIONS] -- argv[0] argv[1]...\n"
        "   -s sec           sec. to sleep before respawning.         (def=1)\n"
        "   -t max           max. consecutive failures before quit.   (def=0, unlimited)\n"
        "   -T max           max. total failures before quit.         (def=0, unlimited)\n"
	    "   -x \"command\"     command/script to run at each respawn.   (def. nothing)\n"
        "   -e               terminate when child exits successfully. (def. always resp)\n"	
	    "   -d               run as daemon\n"
        "     -p             changes the current working directory to the root \"/\"\n"
        "     -c             redirect standard  input, output and error to /dev/null\n"
        "   -k               signal to kill the child. (def. SIGTERM)\n" 
        "   -h               print this help\n";

typedef struct { 
    const char *name;
    int val;
} sigtype;

#define _sig(x) { #x, x } 
static sigtype signal_type[]= {
    _sig(SIGHUP),
    _sig(SIGINT),
    _sig(SIGQUIT),
    _sig(SIGILL),
    _sig(SIGTRAP),
    _sig(SIGABRT),
    _sig(SIGIOT),
    _sig(SIGBUS),
    _sig(SIGFPE),
    _sig(SIGKILL),
    _sig(SIGUSR1),
    _sig(SIGSEGV),
    _sig(SIGUSR2),
    _sig(SIGPIPE),
    _sig(SIGALRM),
    _sig(SIGTERM),
    _sig(SIGSTKFLT),
    _sig(SIGCLD),
    _sig(SIGCHLD),
    _sig(SIGCONT),
    _sig(SIGSTOP),
    _sig(SIGTSTP),
    _sig(SIGTTIN),
    _sig(SIGTTOU),
    _sig(SIGURG),
    _sig(SIGXCPU),
    _sig(SIGXFSZ),
    _sig(SIGVTALRM),
    _sig(SIGPROF),
    _sig(SIGWINCH),
    _sig(SIGPOLL),
    _sig(SIGIO),
    _sig(SIGPWR),
    _sig(SIGSYS),
    _sig(SIGUNUSED),
};


int   res_sec = 1;	/* sec before respawning */
int   res_cf;		/* consecutive failures  */
int   res_tf;		/* total failures */
int   res_ex; 		/* terminate when child exits succefully */
int   res_chdir;
int   res_redirect;
int   res_daemon;
int   res_signal = SIGTERM;
char *res_child;
char *res_helper;

int  pid;
char file_pid[256] = "/var/run/";

#define log(arg...)	do { 				\
	if (res_daemon)					\
		syslog(log_facility|LOG_INFO,## arg);	\
	else						\
		fprintf(stderr,## arg);			\
} while (0)


void usage()
{
	printf(usage_str,__progname);	
	exit(0);
}

void exit_rt(int i)
{
	log("*** killing %s(%d) (request by user)", res_child , pid);

    if ( kill(pid,res_signal) == -1 && errno == ESRCH)
        log("the pid does not exist (zombie?!?)");

	unlink(file_pid);
	log("%s exits (goodbye)", __progname); 
	exit (0);
}

int getsignum(const char *sig) {
    int ret = -1, i;

    for (i=0; i < sizeof(signal_type)/sizeof(signal_type[0]); i++) {
        if (!strcmp(sig,signal_type[i].name)) {
            ret = signal_type[i].val;
            break;
        }
    }

    return ret;
}

void respawn(int argc, char *argv[], char *envp[])
{
	int status;
	int t=0, cf=0, tf=0;
	FILE *f;

        /* save the pid */
        f = fopen(file_pid, "w");
        if (f != NULL) {
                fprintf(f,"%d",getpid());    
                fclose (f);
        }

	for(;; sleep(res_sec),t++) {

		pid = fork();
		if (pid==-1)
			err(1,"fork");

		if (pid==0) {
			if (execve (argv[0],argv,envp) == -1) {
				warn("execve(%s,...",argv[0]);
			        exit(-1);	
			}
	
		}
	
		log("process %d created.", pid);
		waitpid(pid,&status,0);	

        if (WIFEXITED(status)) {
            log("*** Child exited with: %d", WEXITSTATUS(status));
        }

		if (WIFEXITED(status) && WEXITSTATUS(status)==0) {
			cf = 0;
			if (res_ex) {
				log("child-exit-status=0; bye.");
				exit(0);
			}
			goto restart;
		}
		else {
			cf++; tf++;
		}	

		if (WIFSIGNALED(status)) {

            log("Child killed by signal %d", WTERMSIG(status));

            if (WTERMSIG(status) == SIGRTMIN || 
                WTERMSIG(status) == SIGTERM ) {
				log("%s exits (goodbye)", __progname); 
                unlink(file_pid);
                exit(0);
            }

		}

		if (res_tf && (res_tf < tf)) {
			log("Max. total failures reached; exit forced.");		
			exit(1);
		}

		if (res_cf && (res_cf < cf)) {
			log("Max. consec. failures reached; exit forced.");		
			exit(2);
		}

	restart:

		if (res_helper) 
			system(res_helper);
		
		log("#%d/%d/%d: Running %s in %d sec...",cf,tf,t,argv[0],res_sec);

	}

}


int 
main(int argc, char *argv[], char *envp[])
{
	int i;
	
	while( (i=getopt(argc, argv, "s:t:T:x:k:edpch"))!= EOF)	
	    switch(i) {
		case 's': res_sec = atoi(optarg); break;
		case 't': res_cf  = atoi(optarg); break;
		case 'T': res_tf  = atoi(optarg); break;
		case 'e': res_ex  = 1; break;
		case 'd': res_daemon = 1; break;
		case 'x': res_helper = optarg; break;
        case 'p': res_chdir = 1; break;
        case 'c': res_redirect = 1; break;
        case 'k': {
            res_signal = atoi(optarg); 
            if (res_signal != 0)
                break;
            res_signal = getsignum(optarg);
            if (res_signal == -1)
                errx(1,"unknown signal %s", optarg);
            break;
        }
        case 'h': usage();
	}

	argc -= optind;
	argv += optind;

	if ( argc < 1 )
		usage();

    if (!res_daemon && res_chdir) {
        errx(2, "-p is valid with -d option");
    }
    if (!res_daemon && res_redirect) {
        errx(3, "-c is valid with -d option");
    }

	res_child = argv[0];

	char * r = strrchr(argv[0],'/');

    strcat(file_pid, r ? r+1 : argv[0]);
	strcat(file_pid,".pid");

	fprintf(stderr,"respawn (%s):\n", argv[0]);
	fprintf(stderr,"    resp_sec   : %d\n", res_sec);
	fprintf(stderr,"    consc_fails: %d\n", res_cf);
	fprintf(stderr,"    total_fails: %d\n", res_tf);
	fprintf(stderr,"    resp_helper: %s\n", res_helper);
	fprintf(stderr,"    file_pid   : %s\n", file_pid);
    fprintf(stderr,"    sigkill    : %d\n", res_signal);

    if (res_chdir)
        fprintf(stderr,"    chdir to \"/\" on \n"); 
    if (res_redirect)
        fprintf(stderr,"    redirect std* to /dev/null\n"); 

	signal(SIGHUP,SIG_IGN);
	signal(SIGKILL,SIG_IGN);
	signal(SIGPIPE,SIG_IGN);
	signal(SIGALRM,SIG_IGN);
	signal(SIGTERM,SIG_IGN);
	signal(SIGUSR1,SIG_IGN);
	signal(SIGUSR2,SIG_IGN);
	signal(SIGSTOP,SIG_IGN);
	signal(SIGTSTP,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGTTOU,SIG_IGN);
	signal(SIGPOLL,SIG_IGN);
	signal(SIGPROF,SIG_IGN);
	signal(SIGVTALRM,SIG_IGN);

	signal(SIGRTMIN, exit_rt);
	signal(SIGINT,   exit_rt);
    signal(SIGTERM,  exit_rt);

	openlog(__progname, LOG_CONS|LOG_NDELAY, log_facility);

	if (res_daemon)
		daemon(!res_chdir,!res_redirect);

	respawn(argc, argv, envp);

	return 0;
}	
