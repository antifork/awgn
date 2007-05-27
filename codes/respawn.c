#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <err.h>

extern char *__progname; 

const int log_facility = LOG_LOCAL6;

static const char usage_str[]=
        "Usage: %s [OPTIONS] -- argv[0] argv[1]...\n"
        "   -s sec           sec. to sleep before respawning.         (def=1)\n"
        "   -t max           max. consecutive failures before quit.   (def=0, unlimited)\n"
        "   -T max           max. total failures before quit.         (def=0, unlimited)\n"
	"   -c \"command\"     command/script to run at each respawn.   (def. nothing)\n"
        "   -e               terminate when child exits successfully. (def. always resp)\n"	
	"   -d               run as daemon\n"
        "   -h               print this help\n";

int   res_sec = 1;	/* sec before respawning */
int   res_cf;		/* consecutive failures  */
int   res_tf;		/* total failures */
int   res_ex; 		/* terminate when child exits succefully */
int   res_daemon;
char *res_child;
char *res_helper;

int  pid;
char file_pid[256] = "/var/run/";


void usage()
{
	printf(usage_str,__progname);	
	exit(0);
}


#define log(arg...)	do { 				\
	if (res_daemon)					\
		syslog(log_facility|LOG_INFO,## arg);	\
	else						\
		fprintf(stderr,## arg);			\
} while (0)


void exit_rt(int i)
{
	log("RTMIN: killing %s(%d) (request by user)\n", res_child , pid);
	kill(pid,SIGKILL);
	unlink(file_pid);
	exit (0);
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
	
		log("process %d created.\n", pid);
		waitpid(pid,&status,0);	

		if (WIFEXITED(status) && WEXITSTATUS(status)==0) {
			cf = 0;
			if (res_ex) {
				log("child-exit-status=0; bye.\n");
				exit(0);
			}
			goto restart;
		}
		else {
			cf++; tf++;
		}	

		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGRTMIN) {
			log("Child killed by SIGTRMIN.\n");
			exit_rt(SIGRTMIN);
		}

		if (res_tf && (res_tf < tf)) {
			log("Max. total failures reached; exit forced.\n");		
			exit(1);
		}

		if (res_cf && (res_cf < cf)) {
			log("Max. consec. failures reached; exit forced.\n");		
			exit(2);
		}

	restart:

		if (res_helper) 
			system(res_helper);
		
		log("#%d/%d/%d: Running %s in %d sec...\n",cf,tf,t,argv[0],res_sec);

	}

}


int 
main(int argc, char *argv[], char *envp[])
{
	int i;
	
	while( (i=getopt(argc, argv, "s:t:T:c:edh"))!= EOF)	
	    switch(i) {
		case 's': res_sec = atoi(optarg); break;
		case 't': res_cf  = atoi(optarg); break;
		case 'T': res_tf  = atoi(optarg); break;
		case 'e': res_ex  = 1; break;
		case 'd': res_daemon = 1; break;
		case 'c': res_helper = optarg; break;
		case 'h': usage();
	}

	argc -= optind;
	argv += optind;

	if ( argc < 1 )
		usage();

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

	openlog("respawn", LOG_CONS|LOG_NDELAY, log_facility);

	if (res_daemon)
		daemon(0,0);

	respawn(argc, argv, envp);

	return 0;
}	
