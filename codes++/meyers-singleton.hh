/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef MEYERS_SINGLETON_HH
#define MEYERS_SINGLETON_HH

//
// My best singleton/multiton version, inspired to that of Meyers. It is thread safe 
// when compiled with g++ 4.0 or higher 
//

#define SINGLETON_CTOR(x)  \
    x(const generic::singleton<x> &abc) : generic::singleton<x>(abc)

namespace generic
{
    template <typename T> class base_singleton;
    template <typename T>
    class singleton : public base_singleton<T>
    {
        friend class base_singleton<T>;
        singleton() {}

    public:        
        singleton(const singleton &) {}
    };

    template <typename T>
    struct base_singleton
    {
        // singleton instance...
        //
        static T& instance()
        {
            static T _one_((singleton<T>()));
            return _one_;
        }

        // multitons...
        //
        template <int n>
        static T& instance()
        {
            static T _n_((singleton<T>()));
            return _n_;
        }

    };

}

#endif /* MEYERS_SINGLETON_HH */

