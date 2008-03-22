/* $Id: sprint.hh 1578 2008-03-22 15:40:28Z awgn $ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */
#ifndef SPRINT_HH
#define SPRINT_HH

#include <string>
#include <cstdlib>
#include <macro_template.h>

namespace more {

    static inline std::string sprint(const char *fmt) __attribute__((unused));
    static inline std::string sprint(const char *fmt) 
    {
        char *strp;
        if ( asprintf(&strp, fmt) == -1 )
            return std::string();
        std::string ret(strp); 
        free(strp);
        return ret;
    }

#define aFunction(n)                                \
template < MT_REPEAT_ARG(typename T,n) >            \
static inline std::string sprint(const char *fmt, MT_REPEAT_ARG2(T,arg_,n)) \
{                                                   \
        char *strp; \
        if ( asprintf(&strp, fmt, MT_REPEAT_ARG(arg_,n)) == -1 )  \
            return std::string();   \
        std::string ret(strp); \
        free(strp); \
        return ret; \
}
 
MT_REPEAT_FUNCTION(aFunction,15);

}

#endif /* SPRINT_HH */

