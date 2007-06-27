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

namespace posix {

    class syslog
    {

        private:

            std::stringstream buffer;

            int  _priority;

            int &_facility() {
                static int fac;
                return fac; 
            }

            int &_level() {
                static int lev;
                return lev; 
            }

            syslog (const syslog &);
            syslog & operator= (const syslog &);

        public:

            // option: 	
            //      LOG_CONS	Write directly to system console
            //		LOG_NDELAY	Open the connection immediately
            //		LOG_NOWAIT	Don't wait for child processes
            //		LOG_ODELAY	Connection  is  delayed	
            //		LOG_PERROR	Print to stderr
            //		LOG_PID		Include PID

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

            syslog(const char *ident, int opt = LOG_CONS, int fac = LOG_USER, int lev = LOG_NOTICE ) throw() :
                _priority(0) {

                _facility() = fac;
                _level()    = lev;

                if (ident) 
                    ::openlog(ident,opt,_facility());		
            } 
            
            syslog() : _priority(0) { }

            ~syslog() throw() {
                if (!buffer.str().empty()) {
                    ::syslog( priority() , buffer.str().c_str());
                }
            }

            syslog &setfacility(int fac) throw() {
                _facility() = fac;
                _priority   = 0;
                return *this;
            }

            syslog &setlevel(int lev) throw() {
                _level()  = lev;
                _priority = 0;
                return *this;
            }

            syslog &setlogmask(int mask) throw() {
                ::setlogmask(mask);
                return *this;
            }

            int facility() throw() {
                return _facility();
            }

            int level() throw() {
                return _level();
            }

            int priority() throw() {
                return _priority ? : _facility()| _level();
            }

            // change log level temporary 
            //
            syslog operator()(int lev) {
                syslog ret;
                ret._priority = _facility() | lev;
                return ret;
            }

            // log message 
            //
            template<typename T>
                syslog &operator<<(const T &_m) throw() {
                    buffer << _m;
                    return *this;
                }

            // flush syslog with << std::endl;
            //
            syslog &operator<<( std::ostream& (*_f)(std::ostream&) ) {
                if ( _f == static_cast <std::ostream &(*)(std::ostream &)> (std::endl) ) {
                    ::syslog(priority(), buffer.str().c_str());
                    buffer.str(std::string());  // flush the msg 
                }
                return *this;
            }

    };

}

#endif /* SYSLOG_HH */
