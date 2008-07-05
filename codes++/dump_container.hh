/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef DUMP_CONTAINER_HH
#define DUMP_CONTAINER_HH

#include <iostream>
#include <string>
#include <mtp.hh>

namespace std {
    
    struct dump_container 
    {
        private:
            dump_container(const dump_container &);
            dump_container & operator=(const dump_container &);
        public:
        static char & 
        sep(const char _sep = '\0')
        {
            static __thread char sep;
            return sep;
        }
    };

    struct dump_container_sep 
    {
        char _M_s;
        dump_container_sep(const char _sep = '\0')
        : _M_s(_sep)
        {}
    };

    template <typename T>
    typename mtp::enable_if< traits::is_container<T>::value && !tr1::is_same<T, std::string>::value, std::ostream>::type &
    operator<<(std::ostream &out, const T &v)
    {
        typename T::const_iterator it = v.begin();
        for(; it != v.end()  ; ++it, std::cout << ( it != v.end() ? dump_container::sep() : '\0' )  )
            out << *it;
        return out;
    };

    std::ostream &
    operator<< (std::ostream &out, const dump_container_sep &r)
    {
        dump_container::sep() = r._M_s;
        return out;
    }

}

#endif /* DUMP_CONTAINER_HH */
