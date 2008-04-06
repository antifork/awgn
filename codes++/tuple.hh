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

/* the so-called __VA_NARG__ (PP_NARG) macro from the thread at 
   http://groups.google.com/group/comp.std.c/browse_frm/thread/77ee8c8f92e4a3fb 
 */

#define PP_NARG(...) \
         PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) \
         PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N
#define PP_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0 

#define PASTE(a,b)  a ## b
#define XPASTE(a,b) PASTE(a,b)

// typelist macro helper ala loki...
//

#define TLIST_1(a)                  typelist<a,empty>
#define TLIST_2(a,...)              typelist<a,TLIST_1(__VA_ARGS__) >
#define TLIST_3(a,...)              typelist<a,TLIST_2(__VA_ARGS__) >
#define TLIST_4(a,...)              typelist<a,TLIST_3(__VA_ARGS__) >
#define TLIST_5(a,...)              typelist<a,TLIST_4(__VA_ARGS__) >
#define TLIST_6(a,...)              typelist<a,TLIST_5(__VA_ARGS__) >
#define TLIST_7(a,...)              typelist<a,TLIST_6(__VA_ARGS__) >
#define TLIST_8(a,...)              typelist<a,TLIST_7(__VA_ARGS__) >
#define TLIST_9(a,...)              typelist<a,TLIST_8(__VA_ARGS__) >
#define TLIST(...)                  XPASTE(TLIST_ ,PP_NARG(__VA_ARGS__)) ( __VA_ARGS__) 

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
        typedef typename TL::head          elem_type;
        typedef tuple<typename TL::tail>   list_type;

        elem_type _M_elem;
        list_type _M_list;

    public:
    
        tuple() 
        : _M_elem(),
          _M_list()
        {}

        template <typename T1>
        tuple(const T1 &t1)
        : _M_elem(t1),
          _M_list()
        {}

        template <typename T1, typename T2>
        tuple(const T1 &t1, const T2 &t2)
        :  
          _M_elem(t1),
          _M_list(t2)
        {}

        template <typename T1, typename T2, typename T3>
        tuple(const T1 &t1, const T2 &t2, const T3 &t3)
        :  
          _M_elem(t1),
          _M_list(t2,t3)
        {}

        template <typename T1, typename T2, typename T3,
                  typename T4>
        tuple(const T1 &t1, const T2 &t2, const T3 &t3,
              const T4 &t4)
        :  
          _M_elem(t1),
          _M_list(t2,t3,t4)
        {}

        template <typename T1, typename T2, typename T3,
                  typename T4, typename T5>
        tuple(const T1 &t1, const T2 &t2, const T3 &t3,
              const T4 &t4, const T5 &t5)
        :  
          _M_elem(t1),
          _M_list(t2,t3,t4,t5)
        {}

        template <typename T1, typename T2, typename T3,
                  typename T4, typename T5, typename T6>
        tuple(const T1 &t1, const T2 &t2, const T3 &t3,
              const T4 &t4, const T5 &t5, const T6 &t6)
        :  
          _M_elem(t1),
          _M_list(t2,t3,t4,t5,t6)
        {}

        template <typename T1, typename T2, typename T3,
                  typename T4, typename T5, typename T6,
                  typename T7>
        tuple(const T1 &t1, const T2 &t2, const T3 &t3,
              const T4 &t4, const T5 &t5, const T6 &t6,
              const T7 &t7)
        :  
          _M_elem(t1),
          _M_list(t2,t3,t4,t5,t6,t7)
        {}

        template <typename T1, typename T2, typename T3,
                  typename T4, typename T5, typename T6,
                  typename T7, typename T8>
        tuple(const T1 &t1, const T2 &t2, const T3 &t3,
              const T4 &t4, const T5 &t5, const T6 &t6,
              const T7 &t7, const T8 &t8)
        :  
          _M_elem(t1),
          _M_list(t2,t3,t4,t5,t6,t7,t8)
        {}

        template <typename T1, typename T2, typename T3,
                  typename T4, typename T5, typename T6,
                  typename T7, typename T8, typename T9>
        tuple(const T1 &t1, const T2 &t2, const T3 &t3,
              const T4 &t4, const T5 &t5, const T6 &t6,
              const T7 &t7, const T8 &t8, const T9 &t9)
        :  
          _M_elem(t1),
          _M_list(t2,t3,t4,t5,t6,t7,t8,t9)
        {}

        template <int n>
        typename add_reference<typename tl_at<TL,n>::type_val >::type_val
        get()
        { return __get(int2Type<n>()); }

        // __get is overloaded by means of int2Type
        template <int n>
        typename add_reference<typename tl_at<TL,n>::type_val >::type_val
        __get(int2Type<n>)
        { return _M_list.__get(int2Type<n-1>()); }

        typename add_reference<elem_type>::type_val
        __get(int2Type<0>)
        { return _M_elem; }

    };

    template<>
    class tuple<empty> {};

    template <typename T1>
    static inline tuple<TLIST(T1)> 
    make_tuple (const T1& a1) 
    { 
        return tuple<TLIST(T1)> (a1); 
    }    
    
    template <typename T1, typename T2>
    static inline tuple<TLIST(T1,T2)> 
    make_tuple (const T1& a1, const T2& a2) 
    { 
        return tuple<TLIST(T1,T2)> (a1,a2); 
    }

    template <typename T1, typename T2, typename T3>
    static inline tuple<TLIST(T1,T2,T3)> 
    make_tuple (const T1& a1, const T2& a2, const T3& a3) 
    { 
        return tuple<TLIST(T1,T2,T3)> (a1,a2,a3); 
    }

    template <typename T1, typename T2, typename T3,
              typename T4>
    static inline tuple<TLIST(T1,T2,T3,T4)> 
    make_tuple (const T1& a1, const T2& a2, const T3& a3, 
                const T4& a4) 
    {
        return tuple<TLIST(T1,T2,T3,T4)> 
                (a1,a2,a3,a4);
    }    
    
    template <typename T1, typename T2, typename T3,
              typename T4, typename T5>
    static inline tuple<TLIST(T1,T2,T3,T4,T5)> 
    make_tuple (const T1& a1, const T2& a2, const T3& a3, 
                const T4& a4, const T5& a5) 
    {
        return tuple<TLIST(T1,T2,T3,T4,T5)> 
                (a1,a2,a3,a5,a5);
    }

    template <typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6>
    static inline tuple<TLIST(T1,T2,T3,T4,T5,T6)> 
    make_tuple (const T1& a1, const T2& a2, const T3& a3, 
                const T4& a4, const T5& a5, const T6& a6) 
    {
        return tuple<TLIST(T1,T2,T3,T4,T5,T6)> 
                (a1,a2,a3,a4,a5,a6); 
    }

    template <typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6,
              typename T7>
    static inline tuple<TLIST(T1,T2,T3,T4,T5,T6,T7)> 
    make_tuple (const T1& a1, const T2& a2, const T3& a3, 
                const T4& a4, const T5& a5, const T6& a6,
                const T7& a7) 
    {
        return tuple<TLIST(T1,T2,T3,T4,T5,T6,T7)> 
                (a1,a2,a3,a4,a5,a6,a7);
    }

    template <typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6,
              typename T7, typename T8>
    static inline tuple<TLIST(T1,T2,T3,T4,T5,T6,T7,T8)> 
    make_tuple (const T1& a1, const T2& a2, const T3& a3, 
                const T4& a4, const T5& a5, const T6& a6, 
                const T7& a7, const T8& a8) 
    {
        return tuple<TLIST(T1,T2,T3,T4,T5,T6,T7,T8)> 
                (a1,a2,a3,a4,a5,a6,a7,a8);
    }

    template <typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6,
              typename T7, typename T8, typename T9>
    static inline tuple<TLIST(T1,T2,T3,T4,T5,T6,T7,T8,T9)> 
    make_tuple (const T1& a1, const T2& a2, const T3& a3, 
                const T4& a4, const T5& a5, const T6& a6, 
                const T7& a7, const T8& a8, const T9& a9) 
    {
        return tuple<TLIST(T1,T2,T3,T4,T5,T6,T7,T8,T9)> 
                (a1,a2,a3,a4,a5,a6,a7,a8,a9);
    }
}

#endif /* TUPLE_HH */
