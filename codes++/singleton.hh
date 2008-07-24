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
    x(const tag &abc) : generic::singleton<x>(abc)

#define MULTITON_CTOR(x,...)  \
    x(const tag &abc, ## __VA_ARGS__) : generic::singleton<x>(abc)

namespace generic
{
    template <typename T>
    struct singleton
    {

    protected:
        struct tag {
            friend class singleton<T>;
        private:
            tag() 
            {}
            ~tag() 
            {}
            tag(const tag&) 
            {}
            const tag& operator=(const tag&) 
            {}
        };

    public:
        // singleton instance...
        //
        static volatile T& instance()
        {
            static volatile T _one_((tag()));
            return _one_;
        }

        // multitons...
        //
        template <int n>
        static volatile T& instance()
        {
            static volatile T _n_((tag()));
            return _n_;
        }
        template <int n, typename U>
        static volatile T& instance(const U &u = U())
        {
            static volatile T _n_((tag()), u);
            return _n_;
        }
        template <int n, typename U, typename V>
        static volatile T& instance(const U &u = U(), const V &v = V())
        {
            static volatile T _n_((tag()), u, v);
            return _n_;
        }
        template <int n, typename U, typename V, typename W>
        static volatile T& instance(const U &u = U(), const V &v = V(), const W &w = W())
        {
            static volatile T _n_((tag()), u, v, w);
            return _n_;
        }
        template <int n, typename U, typename V, typename W, typename X>
        static volatile T& instance(const U &u = U(), const V &v = V(), const W &w = W(), 
                           const X &x = X())
        {
            static volatile T _n_((tag()), u, v, w, x);
            return _n_;
        }
        template <int n, typename U, typename V, typename W, typename X, typename Y>
        static volatile T& instance(const U &u = U(), const V &v = V(), const W &w = W(), 
                           const X &x = X(), const Y &y = Y())
        {
            static volatile T _n_((tag()), u, v, w, x, y);
            return _n_;
        }
        template <int n, typename U, typename V, typename W, typename X, typename Y, typename Z>
        static volatile T& instance(const U &u = U(), const V &v = V(), const W &w = W(), 
                           const X &x = X(), const Y &y = Y(), const Z &z = Z())
        {
            static volatile T _n_((tag()), u, v, w, x, y, z);
            return _n_;
        }

        singleton(const tag &) 
        {}

        virtual ~singleton()
        {}

    private:
        singleton();
        singleton(const singleton&);                // noncopyable
        singleton &operator=(const singleton &);    // noncopyable  

    };

}

#endif /* MEYERS_SINGLETON_HH */

