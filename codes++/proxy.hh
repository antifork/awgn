/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#ifndef PROXY_HH
#define PROXY_HH

namespace generic 
{
    template <typename T,int N>
    class proxy {

        T _M_val;

    public:

#ifdef USE_EXPLICIT
        explicit 
#endif        
        proxy(T v = T())
        : _M_val(v)
        {}

        proxy(const proxy &rhs)
        { _M_val = rhs._M_val; }

        proxy & operator=(const proxy &rhs)
        { _M_val = rhs._M_val; }

        proxy & operator=(const T &val)
        { _M_val = val; }

        // exposing internals requires 
        // both const and non const methods

        T & get()
        { return _M_val; }

        const T & get() const
        { return _M_val; }

#ifndef USE_EXPLICIT
        operator T() const
        { return _M_val; }
#endif
        T *
        operator &()
        { return &_M_val; }

        const T *
        operator &() const
        { return &_M_val; }
    };
}

#endif /* PROXY_HH */
