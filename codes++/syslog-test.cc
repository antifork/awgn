#include <iostream>
#include <string>

#include <syslog.hh>

extern char *__progname;


int main(int argc, char **argv)
{
    posix::syslog yourlog("pippo"); {
        posix::syslog mylog;
        mylog.setpriority(LOG_LOCAL0|LOG_INFO);
        mylog << "prova" << std::endl;
    }
   
    yourlog << "ok" << std::endl;
}
