#include <iostream>
#include <string>

#include <syslog.hh>

extern char *__progname;


int main(int argc, char **argv)
{
    posix::syslog yourlog("syslog.cc"); 
    
    {
        posix::syslog mylog;
        mylog.setpriority(LOG_LOCAL0|LOG_INFO);
        mylog << "hello world!" << std::endl;
    }
   
    yourlog << "done." << std::endl;
}
