/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#include <syslog.hh>

namespace sys
{
    sys::syslog<LOG_NDELAY>                     log;
    sys::syslog<LOG_NDELAY|LOG_PID>             plog;
    sys::syslog<LOG_NDELAY|LOG_PERROR>          err;
    sys::syslog<LOG_NDELAY|LOG_PERROR|LOG_PID>  perr;
}

