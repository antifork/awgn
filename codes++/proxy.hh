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
    //
    // to be used in place of pure typedef... 
    // define USE_EXPLICIT to enforce typechecking
    //
    // usage: typedef proxy<int, 0> my_int;
    //        typedef proxy<int, 1> your_int;
    //

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

    // to detect writers...
    //

    template <typename T, int N = 0>
    class write_enable
    {
        T _M_val;
    public:

        explicit 
        write_enable(T v = T())
        : _M_val(v)
        {}

        write_enable &
        operator=(const write_enable &rhs)
        { 
            _M_val = rhs._M_val; 
            return *this;
        }

        const T *
        operator &() const
        { return &_M_val; }

        operator const T &()
        { return _M_val; }

    };

}

#endif /* PROXY_HH */
