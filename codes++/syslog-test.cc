/* $Id$ */
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
    sys::log.open("[log]");
    sys::log << "1 - hello world" << std::endl;

    sys::plog.open("[plog]");
    sys::plog << "2 - hello world" << std::endl;

    sys::err.open("[err]");
    sys::err << "3 - hello world" << std::endl;

    sys::perr.open("[perr]");
    sys::perr << "4 - hello world" << std::endl;
    sys::perr(LOG_EMERG) << "5 - alert message!!!" << std::endl;
    sys::perr << "6 - hello world" << std::endl;
}
