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
#include <pthread.h>
#include <signal.h>

namespace posix 
{
    class pthread 
    {
        pthread_t thread;
        pthread_attr_t *attr;

        pthread(const pthread &);               // noncopyable
        pthread &operator=(const pthread &);    // noncopyable

    public: 
        explicit pthread(pthread_attr_t *a = NULL) :
        thread(),
        attr(a)
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
            if (::pthread_create(&thread, attr, start_routine, this ) != 0)
                throw std::runtime_error("pthread_create");
        }

        void cancel()
        {
            // std::clog << __PRETTY_FUNCTION__ << std::endl;
#ifdef ENABLE_CANCEL
            ::pthread_cancel(thread);
            while (::pthread_cancel(thread)==0) {
                std::clog << "pthread_cancel(" << thread << ") spinning...\n";
                usleep(1000000);
            }
#endif
        }

        int 
        join(void **thread_return=NULL) const 
        { return ::pthread_join(thread, thread_return); }

        int 
        detach() const 
        { return ::pthread_detach(thread); }

        int 
        setschedparam(int policy, const struct sched_param *param)  
        { return ::pthread_setschedparam(thread, policy, param); }

        int 
        getschedparam(int *policy, struct sched_param *param) const 
        { return ::pthread_getschedparam(thread, policy, param); }

        pthread_t 
        id() const 
        { return thread; }

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

    };

    class base_lock
    {
    public:
        enum { reader = 1, writer = 2 };

        static __thread int lock_cnt;

        base_lock()
        {}

    private:
        base_lock(const base_lock&);                        // disable copy constructor
        base_lock& operator=(const base_lock&);             // disable operator= 
        void* operator new(std::size_t);                    // disable heap allocation
        void* operator new[](std::size_t);                  // disable heap allocation
        void operator delete(void*);                        // disable delete operation
        void operator delete[](void*);                      // disable delete operation
        base_lock* operator&();                             // disable address taking
    };

    template <class M, int N=0>
    class scoped_lock : public base_lock 
    {
    public:
        typedef M mutex_type;

        scoped_lock(mutex_type &m) :
        _cs_old(0),
        _mutex(m)
        {
#ifdef ENABLE_CANCEL
            if ( !base_lock::lock_cnt++ ) {
                ::pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&_cs_old);
                // std::clog << pthread_self() << " !going to non cancellable state!\n";
            }
#endif
            _mutex.lock();                
        }

        ~scoped_lock()
        {
            _mutex.unlock();
#ifdef ENABLE_CANCEL
            if (!--base_lock::lock_cnt) {
                ::pthread_setcancelstate(_cs_old,NULL);
                // std::clog << pthread_self() << " !going back to previous state!\n";
            }
#endif
        }

    private:
        int _cs_old;
        mutex_type &_mutex;
    };

    template <class M>
    class scoped_lock<M,base_lock::reader> 
    {
    public:
        typedef M mutex_type;

        scoped_lock(mutex_type &m) :
        _cs_old(0),
        _mutex(m)
        {
#ifdef ENABLE_CANCEL
            if ( !base_lock::lock_cnt++ ) {
                ::pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&_cs_old);
                // std::clog << pthread_self() << " !going to non cancellable state!\n";
            }
#endif
            _mutex.rdlock();                
        }

        ~scoped_lock()
        {
            _mutex.unlock();
#ifdef ENABLE_CANCEL
            if (!--base_lock::lock_cnt) {
                ::pthread_setcancelstate(_cs_old,NULL);
                // std::clog << pthread_self() << " !going back to previous state!\n";
            }
#endif
        }

    private:
        int _cs_old;
        mutex_type &_mutex;
    };

    template <class M>
    class scoped_lock<M,base_lock::writer> 
    {
    public:
        typedef M mutex_type;

        scoped_lock(mutex_type &m) :
        _cs_old(0),
        _mutex(m)
        {
#ifdef ENABLE_CANCEL
            if ( !base_lock::lock_cnt++ ) {
                ::pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&_cs_old);
                // std::clog << pthread_self() << " !going to non cancellable state!\n";
            }
#endif
            _mutex.wrlock();                
        }

        ~scoped_lock()
        {
            _mutex.unlock();
#ifdef ENABLE_CANCEL
            if (!--base_lock::lock_cnt) {
                ::pthread_setcancelstate(_cs_old,NULL);
                // std::clog << pthread_self() << " !going back to previous state!\n";
            }
#endif
        }

    private:
        int _cs_old;
        mutex_type &_mutex;
    };


    class mutex
    {
    public:
        explicit mutex(pthread_mutexattr_t *attr = NULL) : 
        pm()
        {
            if (pthread_mutex_init(&pm,attr) != 0)
                throw std::runtime_error("pthread_mutex_init");
        }
        ~mutex()
        {
            if (pthread_mutex_destroy(&pm) != 0)
                throw std::runtime_error("pthread_mutex_destroy");
        }
        void lock()
        {
            if (::pthread_mutex_lock(&pm) !=0) 
                throw std::runtime_error("pthread_mutex_lock");
        }
        void unlock()
        { 
            if ( ::pthread_mutex_unlock(&pm) != 0)
                throw std::runtime_error("pthread_mutex_unlock");
        }
    private:
        pthread_mutex_t pm;
    };

    class rw_mutex 
    {
    public:
        explicit rw_mutex(pthread_rwlockattr_t *attr = NULL) :
        pm()
        {
            if ( pthread_rwlock_init(&pm, attr) != 0 )
                throw std::runtime_error("pthread_rwlock_init: ");
        }
        ~rw_mutex() {
            if (pthread_rwlock_destroy(&pm) != 0 )
                throw std::runtime_error("pthread_rwlock_destroy");
        }
        void rdlock()
        {
            if ( pthread_rwlock_rdlock(&pm) != 0 )
                throw std::runtime_error("pthread_rwlock_rdlock");
        }
        void wrlock()
        {
            if ( pthread_rwlock_wrlock(&pm) != 0 )
                throw std::runtime_error("pthread_rwlock_unlock");
        }
        void unlock() {
            if ( pthread_rwlock_unlock(&pm) != 0 )
                throw std::runtime_error("pthread_rwlock_unlock");
        }

    private:
        pthread_rwlock_t pm;
    };

    __thread int base_lock::lock_cnt = 0;

} // namespace posix

#endif /* PTHREADPP_HH */
