/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef PTHREADPP_HH
#define PTHREADPP_HH

#include <iostream>
#include <stdexcept>
#include <stdexcept>

#include <pthread.h>
#include <signal.h>

namespace posix 
{
    class pthread 
    {
        pthread_t _M_thread;
        pthread_attr_t *_M_attr;

        pthread(const pthread &);               // noncopyable
        pthread &operator=(const pthread &);    // noncopyable

    public: 
        explicit pthread(pthread_attr_t *a = NULL) 
        : _M_thread(),
          _M_attr(a)
        {};

        virtual ~pthread() 
        {
            // std::clog << __PRETTY_FUNCTION__ << std::endl;
        }

        virtual void *operator()() = 0;

        friend void *start_routine(void *arg);
        static void *start_routine(void *arg)
        {
            pthread *that = reinterpret_cast<pthread *>(arg);
            return that->operator()();
        }

        void start() 
        {
            if (::pthread_create(&_M_thread, _M_attr, start_routine, this ) != 0)
                throw std::runtime_error("pthread_create");
        }

        void cancel()
        {
            // std::clog << __PRETTY_FUNCTION__ << std::endl;
#ifdef ENABLE_CANCEL
            ::pthread_cancel(_M_thread);
            while (::pthread_cancel(_M_thread)==0) {
                std::clog << "pthread_cancel(" << _M_thread << ") spinning...\n";
                usleep(1000000);
            }
#else
            throw std::runtime_error("compile pthread++ with -DENABLE_CANCEL");
#endif
        }

        int 
        join(void **thread_return=NULL) const 
        { return ::pthread_join(_M_thread, thread_return); }

        int 
        detach() const 
        { return ::pthread_detach(_M_thread); }

        int 
        setschedparam(int policy, const struct sched_param *param)  
        { return ::pthread_setschedparam(_M_thread, policy, param); }

        int 
        getschedparam(int *policy, struct sched_param *param) const 
        { return ::pthread_getschedparam(_M_thread, policy, param); }

        int
        setschedprio(int prio)
        { return ::pthread_setschedprio(_M_thread,prio); }

        pthread_t 
        id() const 
        { return _M_thread; }

    protected: 
        // meaningful only in the thread context -- operator() 
        int
        setcancelstate(int state, int *oldstate)
        { return ::pthread_setcancelstate(state,oldstate); }

        int
        setcanceltype(int type, int *oldtype)
        { return ::pthread_setcanceltype(type,oldtype); }

        int 
        psigmask(int how, const sigset_t * __restrict s, sigset_t * __restrict os)
        { return ::pthread_sigmask(how,s,os); }

        int
        getconcurrency()
        { return ::pthread_getconcurrency(); }

        int
        setconcurrency(int new_level)
        { return ::pthread_setconcurrency(new_level); }

    };

    class base_lock
    {
    public:
        enum { reader = 1, writer = 2 };

        static __thread int _M_lock_cnt;

        base_lock()
        {}

        virtual ~base_lock()
        {}

    private:
        base_lock(const base_lock&);                        // disable copy constructor
        base_lock& operator=(const base_lock&);             // disable operator= 
        void* operator new(std::size_t);                    // disable heap allocation
        void* operator new[](std::size_t);                  // disable heap allocation
        // void operator delete(void*);                     // disable delete operation
        // void operator delete[](void*);                   // disable delete operation
        base_lock* operator&();                             // disable address taking
    };

    template <class M, int N=0>
    class scoped_lock : public base_lock 
    {
    public:
        typedef M mutex_type;

        scoped_lock(mutex_type &m) 
        : _M_cs_old(0),
          _M_mutex(m)
        {
#ifdef ENABLE_CANCEL
            if ( !base_lock::_M_lock_cnt++ ) {
                ::pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&_M_cs_old);
                // std::clog << pthread_self() << " !going to non cancellable state!\n";
            }
#endif
            _M_mutex.lock();                
        }

        ~scoped_lock()
        {
            _M_mutex.unlock();
#ifdef ENABLE_CANCEL
            if (!--base_lock::_M_lock_cnt) {
                ::pthread_setcancelstate(_M_cs_old,NULL);
                // std::clog << pthread_self() << " !going back to previous state!\n";
            }
#endif
        }

    private:
        int _M_cs_old;
        mutex_type &_M_mutex;
    };

    template <class M>
    class scoped_lock<M,base_lock::reader> 
    {
    public:
        typedef M mutex_type;

        scoped_lock(mutex_type &m) 
        : _M_cs_old(0),
          _M_mutex(m)
        {
#ifdef ENABLE_CANCEL
            if ( !base_lock::_M_lock_cnt++ ) {
                ::pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&_M_cs_old);
                // std::clog << pthread_self() << " !going to non cancellable state!\n";
            }
#endif
            _M_mutex.rdlock();                
        }

        ~scoped_lock()
        {
            _M_mutex.unlock();
#ifdef ENABLE_CANCEL
            if (!--base_lock::_M_lock_cnt) {
                ::pthread_setcancelstate(_M_cs_old,NULL);
                // std::clog << pthread_self() << " !going back to previous state!\n";
            }
#endif
        }

    private:
        int _M_cs_old;
        mutex_type &_M_mutex;
    };

    template <class M>
    class scoped_lock<M,base_lock::writer> 
    {
    public:
        typedef M mutex_type;

        scoped_lock(mutex_type &m) 
        : _M_cs_old(0),
          _M_mutex(m)
        {
#ifdef ENABLE_CANCEL
            if ( !base_lock::_M_lock_cnt++ ) {
                ::pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&_M_cs_old);
                // std::clog << pthread_self() << " !going to non cancellable state!\n";
            }
#endif
            _M_mutex.wrlock();                
        }

        ~scoped_lock()
        {
            _M_mutex.unlock();
#ifdef ENABLE_CANCEL
            if (!--base_lock::_M_lock_cnt) {
                ::pthread_setcancelstate(_M_cs_old,NULL);
                // std::clog << pthread_self() << " !going back to previous state!\n";
            }
#endif
        }

    private:
        int _M_cs_old;
        mutex_type &_M_mutex;
    };


    class mutex
    {
    public:
        explicit mutex(pthread_mutexattr_t *attr = NULL) 
        : _M_pm()
        {
            if (pthread_mutex_init(&_M_pm,attr) != 0)
                throw std::runtime_error("pthread_mutex_init");
        }
        ~mutex()
        {
            if (pthread_mutex_destroy(&_M_pm) != 0)
                throw std::runtime_error("pthread_mutex_destroy");
        }
        void lock()
        {
            if (::pthread_mutex_lock(&_M_pm) !=0) 
                throw std::runtime_error("pthread_mutex_lock");
        }
        void unlock()
        { 
            if ( ::pthread_mutex_unlock(&_M_pm) != 0)
                throw std::runtime_error("pthread_mutex_unlock");
        }
    private:
        pthread_mutex_t _M_pm;
    };

    class rw_mutex 
    {
    public:
        explicit rw_mutex(pthread_rwlockattr_t *attr = NULL) 
        : _M_pm()
        {
            if ( pthread_rwlock_init(&_M_pm, attr) != 0 )
                throw std::runtime_error("pthread_rwlock_init: ");
        }
        ~rw_mutex() {
            if (pthread_rwlock_destroy(&_M_pm) != 0 )
                throw std::runtime_error("pthread_rwlock_destroy");
        }
        void rdlock()
        {
            if ( pthread_rwlock_rdlock(&_M_pm) != 0 )
                throw std::runtime_error("pthread_rwlock_rdlock");
        }
        void wrlock()
        {
            if ( pthread_rwlock_wrlock(&_M_pm) != 0 )
                throw std::runtime_error("pthread_rwlock_unlock");
        }
        void unlock() {
            if ( pthread_rwlock_unlock(&_M_pm) != 0 )
                throw std::runtime_error("pthread_rwlock_unlock");
        }

    private:
        pthread_rwlock_t _M_pm;
    };

    __thread int base_lock::_M_lock_cnt = 0;

} // namespace posix

#endif /* PTHREADPP_HH */
