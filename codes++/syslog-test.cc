#include <iostream>
#include <string>

#include <syslog.hh>

extern char *__progname;


int main(int argc, char **argv)
{
    posix::syslog yourlog("syslog.cc"); 
    
    {
        posix::syslog mylog;
        mylog.setlevel(LOG_INFO);
        mylog << "hello world!" << std::endl;
        mylog(LOG_WARNING) << "hello world (2)!" << std::endl;
        mylog << "hello world (3)!" << std::endl;
    }
   
    yourlog << "done." << std::endl;
}
