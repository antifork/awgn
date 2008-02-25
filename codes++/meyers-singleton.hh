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
// My best singleton/multiton implementation, inspired to that of Meyers. It is thread safe 
// when compiled with g++ 4.0 or higher 
//

#define SINGLETON_CTOR(x)  \
    x(const VOID &abc) : generic::singleton<x>(abc)

#define MULTITON_CTOR(x,...)  \
    x(const VOID &abc, ## __VA_ARGS__) : generic::singleton<x>(abc)

namespace generic
{
    template <typename T>
    struct singleton
    {
        struct VOID {
            friend class singleton<T>;
        private:
            VOID() {}
            ~VOID() {}
            VOID(const VOID&) {}
            const VOID& operator=(const VOID&) {}
        };

        // singleton instance...
        //
        static T& instance()
        {
            static T _one_((VOID()));
            return _one_;
        }

        // multitons...
        //
        template <int n>
        static T& instance()
        {
            static T _n_((VOID()));
            return _n_;
        }
        template <int n, typename U>
        static T& instance(const U &u = U())
        {
            static T _n_((VOID()), u);
            return _n_;
        }
        template <int n, typename U, typename V>
        static T& instance(const U &u = U(), const V &v = V())
        {
            static T _n_((VOID()), u, v);
            return _n_;
        }
        template <int n, typename U, typename V, typename W>
        static T& instance(const U &u = U(), const V &v = V(), const W &w = W())
        {
            static T _n_((VOID()), u, v, w);
            return _n_;
        }
        template <int n, typename U, typename V, typename W, typename X>
        static T& instance(const U &u = U(), const V &v = V(), const W &w = W(), 
                           const X &x = X())
        {
            static T _n_((VOID()), u, v, w, x);
            return _n_;
        }
        template <int n, typename U, typename V, typename W, typename X, typename Y>
        static T& instance(const U &u = U(), const V &v = V(), const W &w = W(), 
                           const X &x = X(), const Y &y = Y())
        {
            static T _n_((VOID()), u, v, w, x, y);
            return _n_;
        }
        template <int n, typename U, typename V, typename W, typename X, typename Y, typename Z>
        static T& instance(const U &u = U(), const V &v = V(), const W &w = W(), 
                           const X &x = X(), const Y &y = Y(), const Z &z = Z())
        {
            static T _n_((VOID()), u, v, w, x, y, z);
            return _n_;
        }

        singleton(const VOID&) {}

        virtual ~singleton()
        {}

    private:
        singleton(const singleton&);                // noncopyable
        singleton &operator=(const singleton &);    // noncopyable  

    };

}

#endif /* MEYERS_SINGLETON_HH */

