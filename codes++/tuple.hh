/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#ifndef TUPLE_HH
#define TUPLE_HH

// typelist macro helper ala loki...
//

#define TLIST_1(a)                  typelist<a,empty>
#define TLIST_2(a,b)                typelist<a,TLIST_1(b) >
#define TLIST_3(a,b,c)              typelist<a,TLIST_2(b,c) >
#define TLIST_4(a,b,c,d)            typelist<a,TLIST_3(b,c,d) >
#define TLIST_5(a,b,c,d,e)          typelist<a,TLIST_4(b,c,d,e) >
#define TLIST_6(a,b,c,d,e,f)        typelist<a,TLIST_5(b,c,d,e,f) >
#define TLIST_7(a,b,c,d,e,f,g)      typelist<a,TLIST_6(b,c,d,e,f,g) >
#define TLIST_8(a,b,c,d,e,f,g,h)    typelist<a,TLIST_7(b,c,d,e,f,g,h) >
#define TLIST_9(a,b,c,d,e,f,g,h,i)  typelist<a,TLIST_8(b,c,d,e,f,g,h,i) >

namespace generic 
{
    struct empty {};

    // int2Type
    template <int n>
    struct int2Type 
    {
        enum { value = n };
    };

    // add_reference
    template <typename T>
    struct add_reference 
    {
        typedef T &type_val;
    };
    template <typename T>
    struct add_reference<T&>
    {
        typedef T &type_val; 
    };

    // the typelist element
    template <typename T, typename U>
    struct typelist {
        typedef T head;
        typedef U tail;
    };

    // tl_length<TLIST>::val
    //
    template <class TL> struct tl_lenght;
    template <>
    struct tl_lenght<empty>
    {
        enum { value = 0 };
    };
    template <typename T,typename U>
    struct tl_lenght< typelist<T,U> > 
    {
        enum { value = 1 + tl_lenght<U>::value };
    };

    // tl_at<TLIST>::type_val
    //
    template <class TL, int N>
    struct tl_at
    {
        typedef typename tl_at<typename TL::tail, N-1>::type_val type_val;
    };
    template <class TL>
    struct tl_at<TL,0>
    {
        typedef typename TL::head type_val;
    };

    // my tuple! (inspired to that of Thomas Becker)
    template <typename TL>
    class tuple 
    {
        typedef typename TL::head          head_type;
        typedef tuple<typename TL::tail>   list_type;

        head_type elem;
        list_type list;

    public:
    
        tuple() :
        elem(),
        list()
        {}

        template <int n>
        typename add_reference<typename tl_at<TL,n>::type_val >::type_val
        get()
        {   
            return __get(int2Type<n>());
        }

        // __get is overloaded by means of int2Type
        template <int n>
        typename add_reference<typename tl_at<TL,n>::type_val >::type_val
        __get(int2Type<n>)
        {
            return list.__get(int2Type<n-1>());
        }

        typename add_reference<head_type>::type_val
        __get(int2Type<0>)
        {
            return elem;
        }

    };

    template<>
    class tuple<empty> {};

    template <typename T1>
    static inline tuple<TLIST_1(T1)> 
    make_tuple (const T1& a1) 
    {
        tuple<TLIST_1(T1)> ret;
        ret.template get<0>() = a1;
        return ret;
    }    
    
    template <typename T1, typename T2>
    static inline tuple<TLIST_2(T1,T2)> 
    make_tuple (const T1& a1, const T2& a2) 
    {
        tuple<TLIST_2(T1,T2)> ret;
        ret.template get<0>() = a1;
        ret.template get<1>() = a2;
        return ret;
    }

    template <typename T1, typename T2, typename T3>
    static inline tuple<TLIST_3(T1,T2,T3)> 
    make_tuple (const T1& a1, const T2& a2, const T3& a3) 
    {
        tuple<TLIST_3(T1,T2,T3)> ret;
        ret.template get<0>() = a1;
        ret.template get<1>() = a2;
        ret.template get<2>() = a3;
        return ret;
    }

    template <typename T1, typename T2, typename T3,
              typename T4>
    static inline tuple<TLIST_4(T1,T2,T3,T4)> 
    make_tuple (const T1& a1, const T2& a2, const T3& a3, 
                const T4& a4) 
    {
        tuple<TLIST_4(T1,T2,T3,T4)> ret;
        ret.template get<0>() = a1;
        ret.template get<1>() = a2;
        ret.template get<2>() = a3;
        ret.template get<3>() = a4;
        return ret;
    }    
    
    template <typename T1, typename T2, typename T3,
              typename T4, typename T5>
    static inline tuple<TLIST_5(T1,T2,T3,T4,T5)> 
    make_tuple (const T1& a1, const T2& a2, const T3& a3, 
                const T4& a4, const T5& a5) 
    {
        tuple<TLIST_5(T1,T2,T3,T4,T5)> ret;
        ret.template get<0>() = a1;
        ret.template get<1>() = a2;
        ret.template get<2>() = a3;
        ret.template get<3>() = a4;
        ret.template get<4>() = a5;
        return ret;
    }

    template <typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6>
    static inline tuple<TLIST_6(T1,T2,T3,T4,T5,T6)> 
    make_tuple (const T1& a1, const T2& a2, const T3& a3, 
                const T4& a4, const T5& a5, const T6& a6) 
    {
        tuple<TLIST_6(T1,T2,T3,T4,T5,T6)> ret;
        ret.template get<0>() = a1;
        ret.template get<1>() = a2;
        ret.template get<2>() = a3;
        ret.template get<3>() = a4;
        ret.template get<4>() = a5;
        ret.template get<5>() = a6;
        return ret;
    }

    template <typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6,
              typename T7>
    static inline tuple<TLIST_7(T1,T2,T3,T4,T5,T6,T7)> 
    make_tuple (const T1& a1, const T2& a2, const T3& a3, 
                const T4& a4, const T5& a5, const T6& a6,
                const T7& a7) 
    {
        tuple<TLIST_7(T1,T2,T3,T4,T5,T6,T7)> ret;
        ret.template get<0>() = a1;
        ret.template get<1>() = a2;
        ret.template get<2>() = a3;
        ret.template get<3>() = a4;
        ret.template get<4>() = a5;
        ret.template get<5>() = a6;
        ret.template get<6>() = a7;
        return ret;
    }

    template <typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6,
              typename T7, typename T8>
    static inline tuple<TLIST_8(T1,T2,T3,T4,T5,T6,T7,T8)> 
    make_tuple (const T1& a1, const T2& a2, const T3& a3, 
                const T4& a4, const T5& a5, const T6& a6, 
                const T7& a7, const T8& a8) 
    {
        tuple<TLIST_8(T1,T2,T3,T4,T5,T6,T7,T8)> ret;
        ret.template get<0>() = a1;
        ret.template get<1>() = a2;
        ret.template get<2>() = a3;
        ret.template get<3>() = a4;
        ret.template get<4>() = a5;
        ret.template get<5>() = a6;
        ret.template get<6>() = a7;
        ret.template get<7>() = a8;
        return ret;
    }

    template <typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6,
              typename T7, typename T8, typename T9>
    static inline tuple<TLIST_9(T1,T2,T3,T4,T5,T6,T7,T8,T9)> 
    make_tuple (const T1& a1, const T2& a2, const T3& a3, 
                const T4& a4, const T5& a5, const T6& a6, 
                const T7& a7, const T8& a8, const T9& a9) 
    {
        tuple<TLIST_8(T1,T2,T3,T4,T5,T6,T7,T8)> ret;
        ret.template get<0>() = a1;
        ret.template get<1>() = a2;
        ret.template get<2>() = a3;
        ret.template get<3>() = a4;
        ret.template get<4>() = a5;
        ret.template get<5>() = a6;
        ret.template get<6>() = a7;
        ret.template get<7>() = a8;
        ret.template get<8>() = a9;
        return ret;
    }
}

#endif /* TUPLE_HH */
