/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef RWLOCK_POSIX_H
#define RWLOCK_POSIX_H

#include <iostream>
#include <string>
#include <stdexcept>

#include <pthread.h>
#include <errno.h>

// this is a RAII version of the posix rwlock/rwmutex..
//

class RWlock {

    pthread_rwlock_t theLock;
    public:
    
    enum LockType {
        reader,
        writer
    };

    RWlock(pthread_rwlockattr_t *attr = NULL) {
        if ( pthread_rwlock_init(&theLock, attr) != 0 )
            throw std::runtime_error(std::string("pthread_rwlock_init: ").append(strerror(errno)));
    }

    ~RWlock() {
        if (pthread_rwlock_destroy(&theLock) != 0 )
            std::clog << "pthread_rwlock_destroy: " << strerror(errno) << std::endl;
    }

    private:

    int rdlock() {
        int r;
        if ( (r=pthread_rwlock_rdlock(&theLock)) != 0 ) {
            std::clog << r << std::endl;
            throw std::runtime_error(std::string("pthread_rwlock_rdlock: ").append(strerror(errno)));
        }
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return r;
    }

    int wrlock() {
        int r;
        if ( (r=pthread_rwlock_wrlock(&theLock)) != 0 )
            throw std::runtime_error(std::string("pthread_rwlock_wrlock: ").append(strerror(errno)));
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return r;
    }

    int unlock() {
        int r;
        if ( (r=pthread_rwlock_unlock(&theLock)) != 0 )
            throw std::runtime_error(std::string("pthread_rwlock_unlock: ").append(strerror(errno)));
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return r;
    }

    public:
    template <RWlock::LockType RW> friend class RWMutex;
};


template <RWlock::LockType RW> 
class RWMutex {

    RWlock &lock;
    public:
        RWMutex(RWlock &x) : lock(x) {
            lock.rdlock();
        }
        ~RWMutex() {
            lock.unlock();
        }
};


template <>
class RWMutex<RWlock::writer> {

    RWlock &lock;
    public:
        RWMutex(RWlock &x) : lock(x) {
            lock.wrlock();
        }
        ~RWMutex() {
            lock.unlock();
        }
};

#endif /* RWLOCK_POSIX_H */

