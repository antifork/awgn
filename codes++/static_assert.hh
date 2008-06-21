/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef STATIC_ASSERT_HH
#define STATIC_ASSERT_HH

#include <macro_template.h>

namespace more
{
    // CTassert ala Loki
    template <bool> struct CTassert;
    template <>
    struct CTassert<true>
    {
        enum { value = true };
    };
}

#ifndef __GXX_EXPERIMENTAL_CXX0X__
#define static_assert(v,...) generic::CTassert<v> XTOKEN(__VA_ARGS__) __attribute__((unused))
#else
#define static_assert(v,...) static_assert(v, XSTR(__VA_ARGS__))
#endif

#endif /* STATIC_ASSERT_HH */
