/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

// metaprogramming: generic metafunctions 
//

#ifndef MTP_HH
#define MTP_HH

#include <type_traits.hh>

#include <iostream>
#include <string>

namespace mtp {

    class null  {};
    struct empty {};

    template <int n>
    struct tag
    {
        enum { value = n };
    };
    template <typename T>
    struct tag_type
    {
        typedef T type;
    };

    // select type...
    //

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

    // paramenter optimization ala Loki (Alexandrescu)
    //

    template <typename U> struct __param 
    {
        typedef const U & type;
    };
    template <typename U> struct __param<U &> 
    {
        typedef U& type;
    };

    template <typename T>
    struct param : select< traits::is_class<T>::value, 
                           typename __param<T>::type, 
                           typename tag_type<T>::type >
    {};

    // enable_if / disable_if ala boost
    // to be used on return type or additional paramenter
    //
    template <bool B, class T = void>
    struct enable_if 
    {
        typedef T type;
    };
    template <class T>
    struct enable_if<false, T> {};

    template <bool B, class T = void>
    struct disable_if 
    {
        typedef T type;
    };
    template <class T>
    struct disable_if<true, T> {};

}

#endif /* MTP_HH */
