/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#ifndef TYPELIST_HH
#define TYPELIST_HH


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

#define TYPELIST_1(a)                  generic::typelist<a,generic::null>
#define TYPELIST_2(a,...)              generic::typelist<a,TYPELIST_1(__VA_ARGS__) >
#define TYPELIST_3(a,...)              generic::typelist<a,TYPELIST_2(__VA_ARGS__) >
#define TYPELIST_4(a,...)              generic::typelist<a,TYPELIST_3(__VA_ARGS__) >
#define TYPELIST_5(a,...)              generic::typelist<a,TYPELIST_4(__VA_ARGS__) >
#define TYPELIST_6(a,...)              generic::typelist<a,TYPELIST_5(__VA_ARGS__) >
#define TYPELIST_7(a,...)              generic::typelist<a,TYPELIST_6(__VA_ARGS__) >
#define TYPELIST_8(a,...)              generic::typelist<a,TYPELIST_7(__VA_ARGS__) >
#define TYPELIST_9(a,...)              generic::typelist<a,TYPELIST_8(__VA_ARGS__) >
#define TYPELIST_10(a,...)             generic::typelist<a,TYPELIST_9(__VA_ARGS__) >
#define TYPELIST_11(a,...)             generic::typelist<a,TYPELIST_10(__VA_ARGS__) >
#define TYPELIST_12(a,...)             generic::typelist<a,TYPELIST_11(__VA_ARGS__) >
#define TYPELIST_13(a,...)             generic::typelist<a,TYPELIST_12(__VA_ARGS__) >
#define TYPELIST_14(a,...)             generic::typelist<a,TYPELIST_13(__VA_ARGS__) >
#define TYPELIST_15(a,...)             generic::typelist<a,TYPELIST_14(__VA_ARGS__) >
#define TYPELIST_16(a,...)             generic::typelist<a,TYPELIST_15(__VA_ARGS__) >
#define TYPELIST_17(a,...)             generic::typelist<a,TYPELIST_16(__VA_ARGS__) >
#define TYPELIST_18(a,...)             generic::typelist<a,TYPELIST_17(__VA_ARGS__) >
#define TYPELIST_19(a,...)             generic::typelist<a,TYPELIST_18(__VA_ARGS__) >
#define TYPELIST_20(a,...)             generic::typelist<a,TYPELIST_19(__VA_ARGS__) >
#define TYPELIST(...)                  XPASTE(TYPELIST_ ,PP_NARG(__VA_ARGS__)) ( __VA_ARGS__) 

namespace generic
{
    class null {};
    struct empty {};

    // the typelist element
    template <typename T, typename U>
    struct typelist {
        typedef T head;
        typedef U tail;
    };

    namespace TL 
    {
        // length<TLIST>::value
        //
        template <class TLIST> struct length;
        template <>
        struct length<null>
        {
            enum { value = 0 };
        };
        template <typename T,typename U>
        struct length< typelist<T,U> >
        {
            enum { value = 1 + length<U>::value };
        };

        // at<TLIST>::type
        //
        template <class TLIST, int N>
        struct at
        {
            typedef typename at<typename TLIST::tail, N-1>::type type;
        };
        template <class TLIST>
        struct at<TLIST,0>
        {
            typedef typename TLIST::head type;
        };

        // append<TLIST,T>::type
        //
        template <class TLIST, typename T> struct append;
        template <>
        struct append<null, null>
        {
            typedef null type;
        };
        template <typename U>
        struct append<null, U>
        {
            typedef TYPELIST(U) type; 
        };
        template <typename H, typename T>
        struct append<null, typelist<H,T> >
        {
            typedef typelist <H,T> type;
        };
        template <typename H, typename T, typename U>
        struct append<typelist<H,T> , U>
        {
            typedef typelist<H, typename append<T, U>::type > type;
        };

        // insert<TLIST,T>::type
        //
        template <class L, typename E> 
        struct insert
        {
            typedef typename append< typelist<E,null>, L>::type type;
        };
        template <typename L, typename H, typename T>
        struct insert<L, typelist<H,T> >
        {
            typedef typename append< typelist<H,T>, L>::type type;
        };

        // indexof<TLIST,T>::value
        //
        template <typename L, typename E> struct indexof;
        template <typename E>
        struct indexof<null, E> 
        {
            enum { value = -1 };
        };
        template <typename T, typename E>
        struct indexof< typelist<E, T>, E > 
        {
            enum { value = 0 };
        };
        template <typename H, typename T, typename E>
        struct indexof< typelist <H, T> , E >
        {
            enum { value = indexof<T, E>::value == -1 ? -1 : 1 + indexof<T,E>::value };
        };

        // has_type<TLIST,T>::type or fail to compile
        //
        template <typename TLIST, typename T>
        struct has_type 
        {
            template <typename V, int n>
            struct valid_type
            {
                typedef V type;
            };

            template <typename V>
            struct valid_type<V,-1>
            {};

            typedef typename valid_type<T, generic::TL::indexof<TLIST,T>::value >::type type;
        };
    }
};

#endif /* TYPELIST_HH */
