/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#include <iostream>
#include <string>

#include <syslog.hh>

extern char *__progname;


int main(int argc, char **argv)
{
    posix::syslog yourlog(__progname); 

    {
        posix::syslog mylog;
        mylog.setpriority(LOG_LOCAL0|LOG_INFO);
        mylog << "hello" << std::endl;
    }
   
    yourlog << "world" << std::endl;
}
