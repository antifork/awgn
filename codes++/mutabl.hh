/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef _MUTABL_HH_
#define _MUTABL_HH_ 

#include <tr1/type_traits>
namespace more
{
    // compile time assert 
    template <bool> struct mutabl_assert;
    template <>
    struct mutabl_assert<true>
    {
        enum { value = true };
    };

    struct mutabl
    {
        virtual ~mutabl() {}

        template <typename T>
        void turn_into(const T & exemplar)
        {
            mutabl_assert< std::tr1::is_polymorphic<T>::value > is_polymorphic_concept __attribute__ ((unused));
            mutabl_assert< std::tr1::is_base_of<mutabl, T>::value > is_base_concept __attribute__((unused));
            *(void **)this = *(void **)& exemplar;
        }
    };

} // namespace more

#endif /* _MUTABL_HH_ */
