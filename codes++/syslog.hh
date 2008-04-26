/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef SYSLOG_HH
#define SYSLOG_HH

#include <iostream>
#include <sstream>
#include <string>

#include <syslog.h>

namespace sys 
{
    // option: 	
    //      LOG_CONS	Write directly to system console
    //		LOG_NDELAY	Open the connection immediately
    //		LOG_NOWAIT	Don't wait for child processes
    //		LOG_ODELAY	Connection  is  delayed	
    //		LOG_PERROR	Print to stderr
    //		LOG_PID		Include PID

    template <int opt>
    class syslog : public std::ostream 
    {
    private:
        mutable std::stringstream _M_buffer;

        int  _M_priority;

        int &_M_facility() {
            static int fac;
            return fac; 
        }

        int &_M_level() {
            static int lev;
            return lev; 
        }

        syslog (const syslog &);                    // noncopyable
        syslog & operator= (const syslog &);        // noncopyable

    public:

        // facility:	
        //      LOG_AUTH	sec/authorization messages (deprecated)
        //		LOG_AUTHPRIV	sec/authorization messages
        // 		LOG_CRON	clock daemon
        //		LOG_DAEMON	system daemons
        // 		LOG_FTP		ftp daemon
        //		LOG_KERN	kernel messages
        //		LOG_LOCAL0 through LOG_LOCAL7
        //		LOG_LPR		line printer subsystem
        //		LOG_MAIL	mail subsystem
        //		LOG_NEWS	USENET news subsystem
        //		LOG_SYSLOG	message by syslog
        //		LOG_USER 	generic (default)
        //		LOG_UUCP	UUCP subsystem

        // level:       
        //      LOG_EMERG	system is unusable
        //		LOG_ALERT 	action must be taken immediately
        //		LOG_CRIT 	critical conditions
        //		LOG_ERR 	error conditions
        //		LOG_WARNING 	warning conditions
        //		LOG_NOTICE 	normal, but significant, condition
        //		LOG_INFO 	informational message
        //		LOG_DEBUG 	debug-level message

        // priority:    facility|level

        explicit syslog(int fac = LOG_USER, int lev = LOG_NOTICE )  
        : _M_buffer(),
        _M_priority(0) 
        {
            _M_facility() = fac;
            _M_level()    = lev;
        } 

        ~syslog()  
        {
            if (!_M_buffer.str().empty()) {
                ::syslog( priority() , _M_buffer.str().c_str());
            }
        }

        syslog &
        open(const char *ident)
        {
            if (ident)
                ::openlog(ident,opt,_M_facility());
            return *this;
        }

        syslog &
        setfacility(int fac)  
        {
            _M_facility() = fac;
            _M_priority   = 0;
            return *this;
        }

        syslog &
        setlevel(int lev)  
        {
            _M_level()  = lev;
            _M_priority = 0;
            return *this;
        }

        syslog &
        setlogmask(int mask)  
        {
            ::setlogmask(mask);
            return *this;
        }

        const int 
        facility() const  
        { return const_cast<syslog *>(this)->_M_facility(); }

        const int 
        level() const  
        { return const_cast<syslog *>(this)->_M_level(); }

        const int 
        priority() const  
        { return _M_priority ? : const_cast<syslog *>(this)->_M_facility() | 
            const_cast<syslog *>(this)->_M_level(); }

        // change log level temporary 
        //
        syslog 
        operator()(int lev)  
        {
            syslog ret;
            ret._M_priority = _M_facility() | lev;
            return ret;
        }

        // log message 
        //
        template<typename T>
        syslog &
        operator<<(const T &_m)  
        {
            _M_buffer << _m;
            return *this;
        }

        // flush syslog with << std::endl;
        //
        syslog &
        operator<<( std::ostream& (*_f)(std::ostream&) ) 
        {
            if ( _f == static_cast <std::ostream &(*)(std::ostream &)> (std::endl) ) {
                ::syslog(priority(), _M_buffer.str().c_str());
                _M_buffer.str(std::string());  // flush the msg 
            }
            return *this;
        }

    };

    extern sys::syslog<LOG_NDELAY>                     log;
    extern sys::syslog<LOG_NDELAY|LOG_PID>             plog;
    extern sys::syslog<LOG_NDELAY|LOG_PERROR>          err;
    extern sys::syslog<LOG_NDELAY|LOG_PERROR|LOG_PID>  perr;
}

#endif /* SYSLOG_HH */
