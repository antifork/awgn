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

#include <cstdio>
#include <iostream>
#include <string>

namespace extra {

    static std::string sprint(const char *fmt) __attribute__((unused));
    static std::string sprint(const char *fmt) 
        {
            char *strp;
            if ( asprintf(&strp, fmt) == -1 )
                return std::string();
            std::string ret(strp); 
            free(strp);
            return ret;
        }
    template <typename T1>
    static  std::string sprint(const char *fmt, T1 arg_1) 
        {
            char *strp;
            if ( asprintf(&strp, fmt, arg_1) == -1 )
                return std::string();
            std::string ret(strp); 
            free(strp);
            return ret;
        }
    template <typename T1, typename T2>
    static std::string sprint(const char *fmt, T1 arg_1, T2 arg_2) 
        {
            char *strp;
            if ( asprintf(&strp, fmt, arg_1, arg_2) == -1 )
                return std::string();
            std::string ret(strp); 
            free(strp);
            return ret;
        }
    template <typename T1, typename T2, typename T3>
    static std::string sprint(const char *fmt, T1 arg_1, T2 arg_2, T3 arg_3) 
        {
            char *strp;
            if ( asprintf(&strp, fmt, arg_1, arg_2, arg_3) == -1 )
                return std::string();
            std::string ret(strp); 
            free(strp);
            return ret;
        }
    template <typename T1, typename T2, typename T3, typename T4>
    static std::string sprint(const char *fmt, T1 arg_1, T2 arg_2, T3 arg_3, T4 arg_4) 
        {
            char *strp;
            if ( asprintf(&strp, fmt, arg_1, arg_2, arg_3, arg_4) == -1 )
                return std::string();
            std::string ret(strp); 
            free(strp);
            return ret;
        }
    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    static std::string sprint(const char *fmt, T1 arg_1, T2 arg_2, T3 arg_3, T4 arg_4, T5 arg_5) 
        {
            char *strp;
            if ( asprintf(&strp, fmt, arg_1, arg_2, arg_3, arg_4, arg_5) == -1 )
                return std::string();
            std::string ret(strp); 
            free(strp);
            return ret;
        }
    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    static std::string sprint(const char *fmt, T1 arg_1, T2 arg_2, T3 arg_3, T4 arg_4, T5 arg_5, T6 arg_6) 
        {
            char *strp;
            if ( asprintf(&strp, fmt, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6) == -1 )
                return std::string();
            std::string ret(strp); 
            free(strp);
            return ret;
        }
    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    static std::string sprint(const char *fmt, T1 arg_1, T2 arg_2, T3 arg_3, T4 arg_4, T5 arg_5, T6 arg_6, T7 arg_7) 
        {
            char *strp;
            if ( asprintf(&strp, fmt, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7) == -1 )
                return std::string();
            std::string ret(strp); 
            free(strp);
            return ret;
        }
    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    static std::string sprint(const char *fmt, T1 arg_1, T2 arg_2, T3 arg_3, T4 arg_4, T5 arg_5, T6 arg_6, T7 arg_7, T8 arg_8) 
        {
            char *strp;
            if ( asprintf(&strp, fmt, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7, arg_8) == -1 )
                return std::string();
            std::string ret(strp); 
            free(strp);
            return ret;
        }
    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    static std::string sprint(const char *fmt, T1 arg_1, T2 arg_2, T3 arg_3, T4 arg_4, T5 arg_5, T6 arg_6, T7 arg_7, T8 arg_8, T9 arg_9) 
        {
            char *strp;
            if ( asprintf(&strp, fmt, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7, arg_8, arg_9) == -1 )
                return std::string();
            std::string ret(strp); 
            free(strp);
            return ret;
        }
    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    static std::string sprint(const char *fmt, T1 arg_1, T2 arg_2, T3 arg_3, T4 arg_4, T5 arg_5, T6 arg_6, T7 arg_7, T8 arg_8, T9 arg_9, T10 arg_10) 
        {
            char *strp;
            if ( asprintf(&strp, fmt, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7, arg_8, arg_9, arg_10) == -1 )
                return std::string();
            std::string ret(strp); 
            free(strp);
            return ret;
        }

}

#endif /* SPRINT_HH */

