/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef SINGLETON_PROC_HH
#define SINGLETON_PROC_HH

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <iostream>
#include <stdexcept>

extern char *__progname;

namespace more 
{

    class singleton_proc 
    {
        char lockf[24];
        int fd, s;

    public:
        singleton_proc(unsigned short port) 
        : lockf(),
          fd(),
          s()
        {
            ::sprintf(lockf,"/tmp/singleton:%d",port);
            fd = open(lockf, O_CREAT|O_EXCL);
            if (fd == -1) 
                throw std::runtime_error
                (std::string("a session of '").append(__progname).append("' is already running."));

            try {

                if ((s = ::socket(AF_INET, SOCK_STREAM, 0)) == -1)
                    throw std::runtime_error("socket");

                int on = 1;
                if (::setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) == -1)
                    throw std::runtime_error("setsockopt");

                struct sockaddr_in addr;
                addr.sin_family      = AF_INET;
                addr.sin_port        = htons(port);
                addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

                if (::bind(s, reinterpret_cast<const struct sockaddr *>(&addr), sizeof(struct sockaddr_in)) == -1) 
                    throw std::runtime_error(std::string("a session of '").append(__progname).append("' is already running."));

                if (::listen(s,1) == -1) 
                    throw std::runtime_error("listen");

                ::close(fd);
                ::unlink(lockf);

            }
            catch(...) {
                ::close(fd);
                ::unlink(lockf);
                throw;
            }
        }

        ~singleton_proc() {
            ::close(fd);
            ::unlink(lockf);
        }
    };

}

#endif /* SINGLETON_PROC_HH */

