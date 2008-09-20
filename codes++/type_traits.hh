/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef TYPE_TRAITS_HH
#define TYPE_TRAITS_HH

#include <tr1/type_traits>
#include <tr1/tuple>

namespace traits 
{
    // traits support 
    //

    template <bool v, typename U, typename V>
    struct __select
    {
        typedef U type;
    };
    template <typename U, typename V>
    struct __select<false, U, V>
    {
        typedef V type;
    };

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

    // the is_container (using SFINAE... Vandevoorde/Josuttis)
    template <typename T>
    class is_container
    {
        typedef char one;
        typedef long two;
        template <typename C> static one test(typename C::iterator *);
        template <typename C> static two test(...);

    public:
        enum { value = sizeof(test<T>(0)) == sizeof(char) };
    };

    // the is_tuple (using SFINAE... Vandevoorde/Josuttis)
    template <typename T>
    class is_tuple
    {
        typedef char one;
        typedef long two;
        template <typename C> static one test(typename std::tr1::tuple_element<0,C>::type *);
        template <typename C> static two test(...);

    public:
        enum { value = sizeof(test<T>(0)) == sizeof(char) };
    };

    // is_metafunction (using SFINAE... Vandevoorde/Josuttis)
    template <typename T>
    class is_metafunction
    {
        typedef char one;
        typedef long two;
        template <typename C> static one test_type(typename C::type *);
        template <typename C> static two test_type(...);
        template <typename C> static one test_value( typename __select<C::value,long,short>::type * );
        template <typename C> static two test_value(...);

    public:
        enum { value = sizeof(test_type<T>(0)) == sizeof(char) || 
                       sizeof(test_value<T>(0)) == sizeof(char) };
    };

}

#endif /* TYPE_TRAITS_HH */
