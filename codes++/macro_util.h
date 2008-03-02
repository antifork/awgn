/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#ifndef _MACRO_UTIL_H_
#define _MACRO_UTIL_H_

#define MU_ARG(x,n)    x ## n
#define MU_CLASS(x,n)   x(n)

#define MU_REPEAT_1(x)  MU_ARG(x,1) 
#define MU_REPEAT_2(x)  MU_REPEAT_1(x) ,  MU_ARG(x,2) 
#define MU_REPEAT_3(x)  MU_REPEAT_2(x) ,  MU_ARG(x,3)
#define MU_REPEAT_4(x)  MU_REPEAT_3(x) ,  MU_ARG(x,4)
#define MU_REPEAT_5(x)  MU_REPEAT_4(x) ,  MU_ARG(x,5)
#define MU_REPEAT_6(x)  MU_REPEAT_5(x) ,  MU_ARG(x,6)
#define MU_REPEAT_7(x)  MU_REPEAT_6(x) ,  MU_ARG(x,7)
#define MU_REPEAT_8(x)  MU_REPEAT_7(x) ,  MU_ARG(x,8)
#define MU_REPEAT_9(x)  MU_REPEAT_8(x) ,  MU_ARG(x,9)
#define MU_REPEAT_10(x) MU_REPEAT_9(x) ,  MU_ARG(x,10)
#define MU_REPEAT_11(x) MU_REPEAT_10(x) , MU_ARG(x,11)
#define MU_REPEAT_12(x) MU_REPEAT_11(x) , MU_ARG(x,12)
#define MU_REPEAT_13(x) MU_REPEAT_12(x) , MU_ARG(x,13)
#define MU_REPEAT_14(x) MU_REPEAT_13(x) , MU_ARG(x,14)
#define MU_REPEAT_15(x) MU_REPEAT_14(x) , MU_ARG(x,15)
#define MU_REPEAT_16(x) MU_REPEAT_15(x) , MU_ARG(x,16)
#define MU_REPEAT_17(x) MU_REPEAT_16(x) , MU_ARG(x,17)
#define MU_REPEAT_18(x) MU_REPEAT_17(x) , MU_ARG(x,18)
#define MU_REPEAT_19(x) MU_REPEAT_18(x) , MU_ARG(x,19)
#define MU_REPEAT_20(x) MU_REPEAT_19(x) , MU_ARG(x,20)

#define MU_REPEAT_ARG2_1(x,y)  MU_ARG(x,1)  MU_ARG(y,1) 
#define MU_REPEAT_ARG2_2(x,y)  MU_REPEAT_ARG2_1(x,y) ,  MU_ARG(x,2)    MU_ARG(y,2)   
#define MU_REPEAT_ARG2_3(x,y)  MU_REPEAT_ARG2_2(x,y) ,  MU_ARG(x,3)    MU_ARG(y,3)
#define MU_REPEAT_ARG2_4(x,y)  MU_REPEAT_ARG2_3(x,y) ,  MU_ARG(x,4)    MU_ARG(y,4)
#define MU_REPEAT_ARG2_5(x,y)  MU_REPEAT_ARG2_4(x,y) ,  MU_ARG(x,5)    MU_ARG(y,5)
#define MU_REPEAT_ARG2_6(x,y)  MU_REPEAT_ARG2_5(x,y) ,  MU_ARG(x,6)    MU_ARG(y,6)
#define MU_REPEAT_ARG2_7(x,y)  MU_REPEAT_ARG2_6(x,y) ,  MU_ARG(x,7)    MU_ARG(y,7)
#define MU_REPEAT_ARG2_8(x,y)  MU_REPEAT_ARG2_7(x,y) ,  MU_ARG(x,8)    MU_ARG(y,8)
#define MU_REPEAT_ARG2_9(x,y)  MU_REPEAT_ARG2_8(x,y) ,  MU_ARG(x,9)    MU_ARG(y,9)
#define MU_REPEAT_ARG2_10(x,y) MU_REPEAT_ARG2_9(x,y) ,  MU_ARG(x,10)   MU_ARG(y,10)
#define MU_REPEAT_ARG2_11(x,y) MU_REPEAT_ARG2_10(x,y) , MU_ARG(x,11)   MU_ARG(y,11)
#define MU_REPEAT_ARG2_12(x,y) MU_REPEAT_ARG2_11(x,y) , MU_ARG(x,12)   MU_ARG(y,12)
#define MU_REPEAT_ARG2_13(x,y) MU_REPEAT_ARG2_12(x,y) , MU_ARG(x,13)   MU_ARG(y,13)
#define MU_REPEAT_ARG2_14(x,y) MU_REPEAT_ARG2_13(x,y) , MU_ARG(x,14)   MU_ARG(y,14)
#define MU_REPEAT_ARG2_15(x,y) MU_REPEAT_ARG2_14(x,y) , MU_ARG(x,15)   MU_ARG(y,15)
#define MU_REPEAT_ARG2_16(x,y) MU_REPEAT_ARG2_15(x,y) , MU_ARG(x,16)   MU_ARG(y,16)
#define MU_REPEAT_ARG2_17(x,y) MU_REPEAT_ARG2_16(x,y) , MU_ARG(x,17)   MU_ARG(y,17)
#define MU_REPEAT_ARG2_18(x,y) MU_REPEAT_ARG2_17(x,y) , MU_ARG(x,18)   MU_ARG(y,18)
#define MU_REPEAT_ARG2_19(x,y) MU_REPEAT_ARG2_18(x,y) , MU_ARG(x,19)   MU_ARG(y,19)
#define MU_REPEAT_ARG2_20(x,y) MU_REPEAT_ARG2_19(x,y) , MU_ARG(x,20)   MU_ARG(y,20)

#define MU_REPEAT_CLASS_1(x)  MU_CLASS(x,1)
#define MU_REPEAT_CLASS_2(x)  MU_REPEAT_CLASS_1(x) ;  MU_CLASS(x,2) 
#define MU_REPEAT_CLASS_3(x)  MU_REPEAT_CLASS_2(x) ;  MU_CLASS(x,3)
#define MU_REPEAT_CLASS_4(x)  MU_REPEAT_CLASS_3(x) ;  MU_CLASS(x,4)
#define MU_REPEAT_CLASS_5(x)  MU_REPEAT_CLASS_4(x) ;  MU_CLASS(x,5)
#define MU_REPEAT_CLASS_6(x)  MU_REPEAT_CLASS_5(x) ;  MU_CLASS(x,6)
#define MU_REPEAT_CLASS_7(x)  MU_REPEAT_CLASS_6(x) ;  MU_CLASS(x,7)
#define MU_REPEAT_CLASS_8(x)  MU_REPEAT_CLASS_7(x) ;  MU_CLASS(x,8)
#define MU_REPEAT_CLASS_9(x)  MU_REPEAT_CLASS_8(x) ;  MU_CLASS(x,9)
#define MU_REPEAT_CLASS_10(x) MU_REPEAT_CLASS_9(x) ;  MU_CLASS(x,10)
#define MU_REPEAT_CLASS_11(x) MU_REPEAT_CLASS_10(x);  MU_CLASS(x,11)
#define MU_REPEAT_CLASS_12(x) MU_REPEAT_CLASS_11(x);  MU_CLASS(x,12)
#define MU_REPEAT_CLASS_13(x) MU_REPEAT_CLASS_12(x);  MU_CLASS(x,13)
#define MU_REPEAT_CLASS_14(x) MU_REPEAT_CLASS_13(x);  MU_CLASS(x,14)
#define MU_REPEAT_CLASS_15(x) MU_REPEAT_CLASS_14(x);  MU_CLASS(x,15)
#define MU_REPEAT_CLASS_16(x) MU_REPEAT_CLASS_15(x);  MU_CLASS(x,16)
#define MU_REPEAT_CLASS_17(x) MU_REPEAT_CLASS_16(x);  MU_CLASS(x,17)
#define MU_REPEAT_CLASS_18(x) MU_REPEAT_CLASS_17(x);  MU_CLASS(x,18)
#define MU_REPEAT_CLASS_19(x) MU_REPEAT_CLASS_18(x);  MU_CLASS(x,19)
#define MU_REPEAT_CLASS_20(x) MU_REPEAT_CLASS_19(x);  MU_CLASS(x,20)

#define MU_REPEAT_ARG(x,n) MU_REPEAT_ ## n(x)
#define MU_REPEAT_ARG2(x,y,n) MU_REPEAT_ARG2_ ## n(x,y)
#define MU_REPEAT_CLASS(x,n) MU_REPEAT_CLASS_ ## n(x)

// example:
//
// #define aCLASS(n)                                   \
// template < MU_REPEAT_ARG(typename T,n) >            \
// struct test                                         \
// {                                                   \
// }
// 
// MU_REPEAT_CLASS(aCLASS,5);

#endif /* _MACRO_UTIL_H_ */

