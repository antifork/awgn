/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

// generic types 
//

#ifndef GENERIC_HH
#define GENERIC_HH

namespace generic {

    template <int v>
    struct Int2type 
    {
        enum { value = v };
    };

    template <typename T>
    struct Type2type 
    {
        typedef T type;
    };

    template <bool v, typename U, typename V>
    struct select 
    {
        typedef U type;
    };
    template <typename U, typename V>
    struct select<false, U, V> 
    {
        typedef V type;
    };

    // CTassert ala Loki
    template <bool> struct CTassert;
    template <>
    struct CTassert<true> { };

    // the is_class (using SFINAE... Vandevoorde/Josuttis)
    template <typename T>
    class is_class 
    {
        typedef char one;
        typedef long two;
        template <typename C> static one test(int C::*);
        template <typename C> static two test(...);

    public:
        enum { value = sizeof(test<T>(0)) == sizeof(char) }; 
    };

    // paramenter optimization ala Loki (Alexandrescu)
    template <typename U> struct __param 
    {
        typedef const U & type;
    };
    template <typename U> struct __param<U &> 
    {
        typedef U& type;
    };

    template <typename T>
    struct parameter {
        typedef typename select< is_class<T>::value, typename __param<T>::type, T>::type type;
    };

    // enable_if / disable_if ala boost (using SFINAE...)
    // to be used on return type or additional paramenter
    //
    template <bool B, class T = void>
    struct enable_if {
        typedef T type;
    };
    template <class T>
    struct enable_if<false, T> {};

    template <bool B, class T = void>
    struct disable_if {
        typedef T type;
    };
    template <class T>
    struct disable_if<true, T> {};

}

#endif /* GENERIC_HH */
