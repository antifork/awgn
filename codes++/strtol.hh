/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#ifndef STRTOL_HH
#define STRTOL_HH

#include <errno.h>

namespace extra {

    static long int strtol(const std::string &s, int base = 0) {

        char *endptr;
        long int r;

        errno = 0;
     
        r = std::strtol(s.c_str(), &endptr, base);
           if ((errno == ERANGE && (r == LONG_MAX || r == LONG_MIN))
                   || (errno != 0 && r == 0)) {
            throw std::runtime_error("strtol");
        }

        if (endptr == s.c_str())
            throw std::runtime_error("strtol: no digits were found!");
        
        if (*endptr != '\0')   
            throw std::runtime_error("strtol: further characters after number...");

        return r;
    }

}

#endif /* STRTOL_HH */
