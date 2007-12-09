/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#ifndef RT_MESSAGE_HH
#define RT_MESSAGE_HH

#include <iostream>
#include <stdexcept>
#include <string>
#include <map>

#include <singleton.hh>
#include <signal.h>
#include <time.h>
#include <errno.h>

class RTMessageSrv : public generic::singleton<RTMessageSrv, memory::Static> {

    struct sigaction oldact[__SIGRTMAX];

    typedef std::map<int, int (*)(int)> handler_map; 
    handler_map sigmap;

    int run(int sig, int val) {
        handler_map::iterator it = sigmap.find(sig); 
        if (it == sigmap.end())
            throw std::runtime_error("unknown handler");
        return it->second(val);
    }

    public:

    static void rt_sigaction (int signum, siginfo_t *info, void *context)
    {
        // std::cout << "***   si_sgno: " << info->si_signo << std::endl;
        // std::cout << "      si_errn: " << info->si_errno << std::endl;
        // std::cout << "      si_code: " << info->si_code  << std::endl;
        // std::cout << "          pid: " << info->si_pid   << std::endl;
        // std::cout << "       si_int: " << info->si_int   << std::endl;

        sigval ret;

        try {
            ret.sival_int = RTMessageSrv::instance().run(signum,info->si_value.sival_int);
        }
        catch(std::exception &e) {
            std::clog << __PRETTY_FUNCTION__ << ": " << e.what() << std::endl;
            ret.sival_int = -1;
        }
        catch(...) {
            ret.sival_int = -1;
        }

        if(sigqueue(info->si_pid, SIGRTMAX, ret) < 0)
            std::clog << "sigqueue: " << strerror(errno) << std::endl;
    }

    RTMessageSrv() {
        struct sigaction act;
        for(int s= SIGRTMIN; s < SIGRTMAX ;s++) {
            act.sa_sigaction = rt_sigaction;
            act.sa_flags = SA_SIGINFO|SA_RESTART;
            sigemptyset(&act.sa_mask);
            sigaction(s, &act,&oldact[s]); 
        }
    }

    ~RTMessageSrv() {
        for(int s=SIGRTMIN; s<= SIGRTMAX ;s++) {
            sigaction(s, &oldact[s],NULL); 
        }
    }

    void regist(int s, int(*fun)(int)) {
        if (sigmap.find(s) != sigmap.end())
            throw std::runtime_error("handler already registered");
        if (s+SIGRTMIN >= SIGRTMAX)
            throw std::out_of_range("rt-message");
        sigmap.insert(std::make_pair(s+SIGRTMIN, fun));                        
    }

};


class RTMessage {

    struct sigaction oldact;
    struct timespec timeo;
    static RTMessage *that;

    pid_t target;
    sigval ret;

    public:

    RTMessage(int pid, int t=1) : target(pid) { 

        timeo.tv_sec = t;
        timeo.tv_nsec = 0;

        struct sigaction act;
        act.sa_sigaction = rt_sigaction;
        act.sa_flags = SA_SIGINFO;
        sigemptyset(&act.sa_mask);
        sigaction(SIGRTMAX, &act,&oldact); 

    }

    ~RTMessage() {
        sigaction(SIGRTMAX, &oldact,NULL); 
    }

    static void rt_sigaction (int signum, siginfo_t *info, void *context)
    {
        if (!that)
            throw std::runtime_error("rt_sigaction");
        that->ret.sival_int = info->si_value.sival_int;
        kill(getpid(), SIGUSR1);
    }

    int send(int msg, int val) {

        if ( msg+SIGRTMIN >= SIGRTMAX )
            throw std::out_of_range("rt-message");

        that = this;

        sigset_t mask, omask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &mask,&omask); 

        sigval v; v.sival_int = val;
        if ( sigqueue(target,msg+SIGRTMIN,v) < 0)
            throw std::runtime_error(std::string("sigqueue: ").append(strerror(errno)));        

        int r;
        while ( (r=sigtimedwait(&mask, NULL, &timeo)) < 0  && errno == EINTR ) {}

        sigprocmask(SIG_SETMASK, &omask, NULL);
        if ( r < 0 ) 
            throw errno;

        that = NULL; 
        return ret.sival_int;
    }    

}; 


#endif /* RT_MESSAGE_HH */
