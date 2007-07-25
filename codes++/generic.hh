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
        struct Int2type {
            enum { value = v };
        };

    template <typename T>
        struct Type2type {
            typedef T type;
        };

    class  NullType  { };
    struct EmptyType { };    

    template <bool v, typename U, typename V>
        struct Select 
        {
            typedef U type;
        };

    template <typename U, typename V>
        struct Select<false, U, V> 
        {
            typedef V type;
        };

    template <bool> struct CTassert;
    template <>
    struct CTassert<true> { };
}

#endif /* GENERIC_HH */
