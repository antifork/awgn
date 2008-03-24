/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#ifndef _MACRO_TEMPLATE_H_
#define _MACRO_TEMPLATE_H_

#define MT_ARG(x,n)    x ## n
#define MT_ENTITY(x,n)   x(n)

#define MT_REPEAT_1(x)  MT_ARG(x,1) 
#define MT_REPEAT_2(x)  MT_REPEAT_1(x) ,  MT_ARG(x,2) 
#define MT_REPEAT_3(x)  MT_REPEAT_2(x) ,  MT_ARG(x,3)
#define MT_REPEAT_4(x)  MT_REPEAT_3(x) ,  MT_ARG(x,4)
#define MT_REPEAT_5(x)  MT_REPEAT_4(x) ,  MT_ARG(x,5)
#define MT_REPEAT_6(x)  MT_REPEAT_5(x) ,  MT_ARG(x,6)
#define MT_REPEAT_7(x)  MT_REPEAT_6(x) ,  MT_ARG(x,7)
#define MT_REPEAT_8(x)  MT_REPEAT_7(x) ,  MT_ARG(x,8)
#define MT_REPEAT_9(x)  MT_REPEAT_8(x) ,  MT_ARG(x,9)
#define MT_REPEAT_10(x) MT_REPEAT_9(x) ,  MT_ARG(x,10)
#define MT_REPEAT_11(x) MT_REPEAT_10(x) , MT_ARG(x,11)
#define MT_REPEAT_12(x) MT_REPEAT_11(x) , MT_ARG(x,12)
#define MT_REPEAT_13(x) MT_REPEAT_12(x) , MT_ARG(x,13)
#define MT_REPEAT_14(x) MT_REPEAT_13(x) , MT_ARG(x,14)
#define MT_REPEAT_15(x) MT_REPEAT_14(x) , MT_ARG(x,15)
#define MT_REPEAT_16(x) MT_REPEAT_15(x) , MT_ARG(x,16)
#define MT_REPEAT_17(x) MT_REPEAT_16(x) , MT_ARG(x,17)
#define MT_REPEAT_18(x) MT_REPEAT_17(x) , MT_ARG(x,18)
#define MT_REPEAT_19(x) MT_REPEAT_18(x) , MT_ARG(x,19)
#define MT_REPEAT_20(x) MT_REPEAT_19(x) , MT_ARG(x,20)

#define MT_REPEAT_ARG2_1(x,y)  MT_ARG(x,1)  MT_ARG(y,1) 
#define MT_REPEAT_ARG2_2(x,y)  MT_REPEAT_ARG2_1(x,y) ,  MT_ARG(x,2)    MT_ARG(y,2)   
#define MT_REPEAT_ARG2_3(x,y)  MT_REPEAT_ARG2_2(x,y) ,  MT_ARG(x,3)    MT_ARG(y,3)
#define MT_REPEAT_ARG2_4(x,y)  MT_REPEAT_ARG2_3(x,y) ,  MT_ARG(x,4)    MT_ARG(y,4)
#define MT_REPEAT_ARG2_5(x,y)  MT_REPEAT_ARG2_4(x,y) ,  MT_ARG(x,5)    MT_ARG(y,5)
#define MT_REPEAT_ARG2_6(x,y)  MT_REPEAT_ARG2_5(x,y) ,  MT_ARG(x,6)    MT_ARG(y,6)
#define MT_REPEAT_ARG2_7(x,y)  MT_REPEAT_ARG2_6(x,y) ,  MT_ARG(x,7)    MT_ARG(y,7)
#define MT_REPEAT_ARG2_8(x,y)  MT_REPEAT_ARG2_7(x,y) ,  MT_ARG(x,8)    MT_ARG(y,8)
#define MT_REPEAT_ARG2_9(x,y)  MT_REPEAT_ARG2_8(x,y) ,  MT_ARG(x,9)    MT_ARG(y,9)
#define MT_REPEAT_ARG2_10(x,y) MT_REPEAT_ARG2_9(x,y) ,  MT_ARG(x,10)   MT_ARG(y,10)
#define MT_REPEAT_ARG2_11(x,y) MT_REPEAT_ARG2_10(x,y) , MT_ARG(x,11)   MT_ARG(y,11)
#define MT_REPEAT_ARG2_12(x,y) MT_REPEAT_ARG2_11(x,y) , MT_ARG(x,12)   MT_ARG(y,12)
#define MT_REPEAT_ARG2_13(x,y) MT_REPEAT_ARG2_12(x,y) , MT_ARG(x,13)   MT_ARG(y,13)
#define MT_REPEAT_ARG2_14(x,y) MT_REPEAT_ARG2_13(x,y) , MT_ARG(x,14)   MT_ARG(y,14)
#define MT_REPEAT_ARG2_15(x,y) MT_REPEAT_ARG2_14(x,y) , MT_ARG(x,15)   MT_ARG(y,15)
#define MT_REPEAT_ARG2_16(x,y) MT_REPEAT_ARG2_15(x,y) , MT_ARG(x,16)   MT_ARG(y,16)
#define MT_REPEAT_ARG2_17(x,y) MT_REPEAT_ARG2_16(x,y) , MT_ARG(x,17)   MT_ARG(y,17)
#define MT_REPEAT_ARG2_18(x,y) MT_REPEAT_ARG2_17(x,y) , MT_ARG(x,18)   MT_ARG(y,18)
#define MT_REPEAT_ARG2_19(x,y) MT_REPEAT_ARG2_18(x,y) , MT_ARG(x,19)   MT_ARG(y,19)
#define MT_REPEAT_ARG2_20(x,y) MT_REPEAT_ARG2_19(x,y) , MT_ARG(x,20)   MT_ARG(y,20)

#define MT_REPEAT_FUNCTION_1(x)  MT_ENTITY(x,1)
#define MT_REPEAT_FUNCTION_2(x)  MT_REPEAT_FUNCTION_1(x) ;  MT_ENTITY(x,2) 
#define MT_REPEAT_FUNCTION_3(x)  MT_REPEAT_FUNCTION_2(x) ;  MT_ENTITY(x,3)
#define MT_REPEAT_FUNCTION_4(x)  MT_REPEAT_FUNCTION_3(x) ;  MT_ENTITY(x,4)
#define MT_REPEAT_FUNCTION_5(x)  MT_REPEAT_FUNCTION_4(x) ;  MT_ENTITY(x,5)
#define MT_REPEAT_FUNCTION_6(x)  MT_REPEAT_FUNCTION_5(x) ;  MT_ENTITY(x,6)
#define MT_REPEAT_FUNCTION_7(x)  MT_REPEAT_FUNCTION_6(x) ;  MT_ENTITY(x,7)
#define MT_REPEAT_FUNCTION_8(x)  MT_REPEAT_FUNCTION_7(x) ;  MT_ENTITY(x,8)
#define MT_REPEAT_FUNCTION_9(x)  MT_REPEAT_FUNCTION_8(x) ;  MT_ENTITY(x,9)
#define MT_REPEAT_FUNCTION_10(x) MT_REPEAT_FUNCTION_9(x) ;  MT_ENTITY(x,10)
#define MT_REPEAT_FUNCTION_11(x) MT_REPEAT_FUNCTION_10(x);  MT_ENTITY(x,11)
#define MT_REPEAT_FUNCTION_12(x) MT_REPEAT_FUNCTION_11(x);  MT_ENTITY(x,12)
#define MT_REPEAT_FUNCTION_13(x) MT_REPEAT_FUNCTION_12(x);  MT_ENTITY(x,13)
#define MT_REPEAT_FUNCTION_14(x) MT_REPEAT_FUNCTION_13(x);  MT_ENTITY(x,14)
#define MT_REPEAT_FUNCTION_15(x) MT_REPEAT_FUNCTION_14(x);  MT_ENTITY(x,15)
#define MT_REPEAT_FUNCTION_16(x) MT_REPEAT_FUNCTION_15(x);  MT_ENTITY(x,16)
#define MT_REPEAT_FUNCTION_17(x) MT_REPEAT_FUNCTION_16(x);  MT_ENTITY(x,17)
#define MT_REPEAT_FUNCTION_18(x) MT_REPEAT_FUNCTION_17(x);  MT_ENTITY(x,18)
#define MT_REPEAT_FUNCTION_19(x) MT_REPEAT_FUNCTION_18(x);  MT_ENTITY(x,19)
#define MT_REPEAT_FUNCTION_20(x) MT_REPEAT_FUNCTION_19(x);  MT_ENTITY(x,20)

#define MT_REPEAT_FUNCTION_1(x)  MT_ENTITY(x,1)
#define MT_REPEAT_FUNCTION_2(x)  MT_REPEAT_FUNCTION_1(x) ;  MT_ENTITY(x,2) 
#define MT_REPEAT_FUNCTION_3(x)  MT_REPEAT_FUNCTION_2(x) ;  MT_ENTITY(x,3)
#define MT_REPEAT_FUNCTION_4(x)  MT_REPEAT_FUNCTION_3(x) ;  MT_ENTITY(x,4)
#define MT_REPEAT_FUNCTION_5(x)  MT_REPEAT_FUNCTION_4(x) ;  MT_ENTITY(x,5)
#define MT_REPEAT_FUNCTION_6(x)  MT_REPEAT_FUNCTION_5(x) ;  MT_ENTITY(x,6)
#define MT_REPEAT_FUNCTION_7(x)  MT_REPEAT_FUNCTION_6(x) ;  MT_ENTITY(x,7)
#define MT_REPEAT_FUNCTION_8(x)  MT_REPEAT_FUNCTION_7(x) ;  MT_ENTITY(x,8)
#define MT_REPEAT_FUNCTION_9(x)  MT_REPEAT_FUNCTION_8(x) ;  MT_ENTITY(x,9)
#define MT_REPEAT_FUNCTION_10(x) MT_REPEAT_FUNCTION_9(x) ;  MT_ENTITY(x,10)
#define MT_REPEAT_FUNCTION_11(x) MT_REPEAT_FUNCTION_10(x);  MT_ENTITY(x,11)
#define MT_REPEAT_FUNCTION_12(x) MT_REPEAT_FUNCTION_11(x);  MT_ENTITY(x,12)
#define MT_REPEAT_FUNCTION_13(x) MT_REPEAT_FUNCTION_12(x);  MT_ENTITY(x,13)
#define MT_REPEAT_FUNCTION_14(x) MT_REPEAT_FUNCTION_13(x);  MT_ENTITY(x,14)
#define MT_REPEAT_FUNCTION_15(x) MT_REPEAT_FUNCTION_14(x);  MT_ENTITY(x,15)
#define MT_REPEAT_FUNCTION_16(x) MT_REPEAT_FUNCTION_15(x);  MT_ENTITY(x,16)
#define MT_REPEAT_FUNCTION_17(x) MT_REPEAT_FUNCTION_16(x);  MT_ENTITY(x,17)
#define MT_REPEAT_FUNCTION_18(x) MT_REPEAT_FUNCTION_17(x);  MT_ENTITY(x,18)
#define MT_REPEAT_FUNCTION_19(x) MT_REPEAT_FUNCTION_18(x);  MT_ENTITY(x,19)
#define MT_REPEAT_FUNCTION_20(x) MT_REPEAT_FUNCTION_19(x);  MT_ENTITY(x,20)

#define MT_REPEAT_ARG(x,n)    MT_REPEAT_ ## n(x)
#define MT_REPEAT_ARG2(x,y,n) MT_REPEAT_ARG2_ ## n(x,y)
#define MT_REPEAT_FUNCTION(x,n)  MT_REPEAT_FUNCTION_ ## n(x)

/* example:

  #define aFunction(n)                                \
  template < MT_REPEAT_ARG(typename T,n) >            \
  int test ()                                         \
  {                                                   \
  }
  
  MT_REPEAT_FUNCTION(aCLASS,5);

*/

/* other useful macros and __VA_NARG__ thread at 
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

#define XSTR_1(a)       #a " "
#define XSTR_2(a,...)   #a " " XSTR_1(__VA_ARGS__)
#define XSTR_3(a,...)   #a " " XSTR_2(__VA_ARGS__)
#define XSTR_4(a,...)   #a " " XSTR_3(__VA_ARGS__)
#define XSTR_5(a,...)   #a " " XSTR_4(__VA_ARGS__)
#define XSTR_6(a,...)   #a " " XSTR_5(__VA_ARGS__)
#define XSTR_7(a,...)   #a " " XSTR_6(__VA_ARGS__)
#define XSTR_8(a,...)   #a " " XSTR_7(__VA_ARGS__)
#define XSTR_9(a,...)   #a " " XSTR_8(__VA_ARGS__)
#define XSTR_10(a,...)  #a " " XSTR_9(__VA_ARGS__)
#define XSTR_11(a,...)  #a " " XSTR_10(__VA_ARGS__)
#define XSTR_12(a,...)  #a " " XSTR_11(__VA_ARGS__)
#define XSTR_13(a,...)  #a " " XSTR_12(__VA_ARGS__)
#define XSTR_14(a,...)  #a " " XSTR_13(__VA_ARGS__)
#define XSTR_15(a,...)  #a " " XSTR_14(__VA_ARGS__)
#define XSTR_16(a,...)  #a " " XSTR_15(__VA_ARGS__)
#define XSTR_17(a,...)  #a " " XSTR_16(__VA_ARGS__)
#define XSTR_18(a,...)  #a " " XSTR_17(__VA_ARGS__)
#define XSTR_19(a,...)  #a " " XSTR_18(__VA_ARGS__)
#define XSTR_20(a,...)  #a " " XSTR_19(__VA_ARGS__)
#define XSTR(...)       XPASTE(XSTR_ ,PP_NARG(__VA_ARGS__)) ( __VA_ARGS__) 

#define XCAT_2(a,b)     a ## b
#define XCAT_3(a,...)   XPASTE(a, XCAT_2(__VA_ARGS__))
#define XCAT_4(a,...)   XPASTE(a, XCAT_3(__VA_ARGS__))
#define XCAT_5(a,...)   XPASTE(a, XCAT_4(__VA_ARGS__))
#define XCAT_6(a,...)   XPASTE(a, XCAT_5(__VA_ARGS__))
#define XCAT_7(a,...)   XPASTE(a, XCAT_6(__VA_ARGS__))
#define XCAT_8(a,...)   XPASTE(a, XCAT_7(__VA_ARGS__))
#define XCAT_9(a,...)   XPASTE(a, XCAT_8(__VA_ARGS__))
#define XCAT_10(a,...)  XPASTE(a, XCAT_9(__VA_ARGS__))
#define XCAT_11(a,...)  XPASTE(a, XCAT_10(__VA_ARGS__))
#define XCAT_12(a,...)  XPASTE(a, XCAT_11(__VA_ARGS__))
#define XCAT_13(a,...)  XPASTE(a, XCAT_12(__VA_ARGS__))
#define XCAT_14(a,...)  XPASTE(a, XCAT_13(__VA_ARGS__))
#define XCAT_15(a,...)  XPASTE(a, XCAT_14(__VA_ARGS__))
#define XCAT_16(a,...)  XPASTE(a, XCAT_15(__VA_ARGS__))
#define XCAT_17(a,...)  XPASTE(a, XCAT_16(__VA_ARGS__))
#define XCAT_18(a,...)  XPASTE(a, XCAT_17(__VA_ARGS__))
#define XCAT_19(a,...)  XPASTE(a, XCAT_18(__VA_ARGS__))
#define XCAT_20(a,...)  XPASTE(a, XCAT_19(__VA_ARGS__))
#define XCAT(...)       XPASTE(XCAT_ ,PP_NARG(__VA_ARGS__)) ( __VA_ARGS__) 


#endif /* _MACRO_TEMPLATE_H_ */

