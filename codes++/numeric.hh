/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#ifndef NUMERIC_HH
#define NUMERIC_HH

namespace numeric
{
    // factorial
    template <int N>
    struct factorial 
    {
        enum { value = N * factorial<N - 1>::value };
    };
    template <>
    struct factorial<0> 
    {
        enum { value = 1 };
    };

    // comb(n,m) = m!/( n! * (m-n)!)
    template <int N, int M>
    struct comb 
    {
        enum { value = factorial<M>::value/(factorial<N>::value * factorial<M-N>::value) };
    };

    // ceil2 ( i <= 2^x )
    template <int i>
    struct ceil2_r 
    {
        enum { value = ceil2_r<i>>1>::value << 1 };
    };
    template <>
    struct ceil2_r<0> 
    {
        enum { value = 1 };
    };
    template <int i>
    struct ceil2 
    {
        enum { value = ceil2_r<(i-1)>>1>::value << 1 };
    };
    template <>
    struct ceil2<0> 
    {
        enum { value = 0 };
    };

    // floor2 ( 2^x <= i )
    template <int i>
    struct floor2_r 
    {
        enum { value = floor2_r<i>>1>::value << 1 };
    };
    template <>
    struct floor2_r<0> 
    {
        enum { value = 1 };
    };
    template <int i>
    struct floor2 
    {
        enum { value = floor2_r<i>>1>::value };
    };
    template <>
    struct floor2<0> 
    {
        enum { value = 0 };
    };

    // sqrt<n>::value >= sqrt(n)
    template <int N, int I=1>
    struct sqrt 
    {
        enum { value = (I*I<N) ? sqrt<N,I+1>::value : I };
    };
    template <int N>
    struct sqrt<N,N> 
    {
        enum { value = N};
    };

    // is_prime 
    template <int p, int i>
    struct is_prime 
    {
        enum { value = (p==2) || (p%i) && is_prime<(i>2?p:0),i-1>::value }; 
    }; 
    template<>
    struct is_prime<0,0> 
    {
        enum { value = 1 };
    }; 
    template<>
    struct is_prime<0,1> 
    {
        enum { value = 1 };
    }; 

    // binary
    template <unsigned long N> 
    struct binary
    {
        enum { value = binary<N/10>::value * 2 + N%10 };
    }; 
    template <>                           
    struct binary<0>                      
    {
        enum { value = 0 };
    };

    // fibonacci
    template<int I>
    struct fibonacci
    {
        enum { value = fibonacci<I-1>::value + fibonacci<I-2>::value };
    };
    template<>
    struct fibonacci<1>
    {
        enum { value = 1 };
    };
    template<>
    struct fibonacci<0>
    {
        enum { value = 1 };
    };

} // namespace numeric

#endif /* NUMERIC_HH */
