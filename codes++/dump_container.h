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
#include <gcc_version.h>

namespace std {
    
    struct dump_container 
    {
        private:
            dump_container(const dump_container &);
            dump_container & operator=(const dump_container &);
        public:

        static char & 
        sep(const char _sep = char())
        {
            static __thread char sep;
            return sep;
        }
    };

    // to specify the char separator...
    //

    struct dump_container_sep 
    {
        char _M_s;
        explicit dump_container_sep(const char &_sep = char())
        : _M_s(_sep)
        {}
    };

    // printable char traits
    //

    template <typename T>
    struct char_mangling_traits 
    { typedef T type; };
    template <>
    struct char_mangling_traits<char>
    { typedef std::string type; };
    template <>
    struct char_mangling_traits<unsigned char>
    { typedef std::string type; };

    template <typename T>
    static inline typename char_mangling_traits<T>::type char_mangling(const T &t)
    { return t; }

    template <>
    #if GCC_VERSION < 40300
    static 
    #endif
    inline
    char_mangling_traits<char>::type char_mangling<char>(const char &c)
    {
        char buf[8];
        sprintf(buf, (c > 31 && c < 127) ? "%c" : "0x%x", c);
        return buf;
    }
    template <>
    #if GCC_VERSION < 40300
    static 
    #endif
    inline
    char_mangling_traits<unsigned char>::type char_mangling<unsigned char>(const unsigned char &c)
    {
        char buf[8];
        sprintf(buf, (c > 31 && c < 127) ? "%c" : "0x%x", c);
        return buf;
    }

    template <typename T>
    typename mtp::enable_if< traits::is_container<T>::value && !tr1::is_same<T, std::string>::value, std::ostream>::type &
    operator<<(std::ostream &out, const T &v)
    {
        typename T::const_iterator it = v.begin();
        for(; it != v.end()  ; ++it, std::cout << ( it != v.end() ? dump_container::sep() : '\0' )  )
            out << char_mangling(*it);
        return out;
    };

    inline std::ostream &
    operator<< (std::ostream &out, const dump_container_sep &r)
    {
        dump_container::sep() = r._M_s;
        return out;
    }

}

#endif /* DUMP_CONTAINER_HH */
