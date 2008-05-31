/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#include <pid-utils.h>

int main(int argc, char *argv[])
{
    char process[256], lk[32];

    printf("get_pid(\"xterm\", ...) test:\n");

    pid_t pid = 0;

    for( ; (pid = get_pid("xterm",pid)) && pid != 0; ) {
        printf("   xterm[%d]\n", pid); 
    }

    pid = getpid();
    printf("get_parent(%d) recursive test:\n", pid);

    do {
         snprintf(lk, sizeof(lk), "/proc/%d/exe",pid);
         int n = readlink(lk,process,sizeof(process)-1);
         if ( n == -1)
             perror("readlink");

         process[ MIN(n, sizeof(process)-1) ]='\0';
         printf("    %s[%d]\n",process, pid);
         
    }

    while ( (pid = get_parent(pid)) != 0); 

    pid = getpid();
    printf("get_sibling(%d,...) recursive test:\n", pid);

    pid_t sib = 0;
    while ( (sib =get_sibling(pid,sib)) != 0 )
    {
        printf("   sibling:%d\n",sib);
    }
 
    pid = getppid();
    printf("get_child(%d,...) recursive test:\n", pid);

    pid_t ch = 0;
    while ( (ch =get_child(pid,ch)) != 0 )
    {
        printf("   child:%d\n",ch);
    }
   
    return 0;
}
