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

// example:
//
// #define aFunction(n)                                \
// template < MT_REPEAT_ARG(typename T,n) >            \
// int test ()                                         \
// {                                                   \
// }
// 
// MT_REPEAT_FUNCTION(aCLASS,5);

#endif /* _MACRO_TEMPLATE_H_ */

