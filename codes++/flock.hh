/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#ifndef FLOCK_HH
#define FLOCK_HH

#include <sys/file.h>
#include <errno.h>

#include <iostream>
#include <stdexcept>

class ScopedFlock
{
public:
    explicit ScopedFlock(const char *filelock, int operation=LOCK_EX) :
    _M_fd(),
    _M_status(false)
    {
        _M_fd = open(filelock, O_RDONLY|O_CREAT);
        if (_M_fd == -1) {
            std::clog << "open: " << strerror(errno) << std::endl;
            throw std::runtime_error("couldn't open file");
        }
        if (flock(_M_fd, operation) < 0 ) {
            std::clog << "flock: " << strerror(errno) << std::endl;
            return;
        }
        _M_status = true;
    }

    ~ScopedFlock()
    {
        if (flock(_M_fd, LOCK_UN) < 0 ) {
            std::clog << "flock(...,LOCK_UN: " << strerror(errno) << std::endl;
            return;
        }
        close(_M_fd);
    }

    bool locked() const 
    { return _M_status; }

private:
    int  _M_fd;
    bool _M_status;
};

#endif /* FLOCK_HH */
