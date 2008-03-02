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

#define MU_ENUM(x,n)    x ## n

#define MU_REPEAT_1(x)  MU_ENUM(x,1) 
#define MU_REPEAT_2(x)  MU_REPEAT_1(x) , MU_ENUM(x,2) 
#define MU_REPEAT_3(x)  MU_REPEAT_2(x) , MU_ENUM(x,3)
#define MU_REPEAT_4(x)  MU_REPEAT_3(x) , MU_ENUM(x,4)
#define MU_REPEAT_5(x)  MU_REPEAT_4(x) , MU_ENUM(x,5)
#define MU_REPEAT_6(x)  MU_REPEAT_5(x) , MU_ENUM(x,6)
#define MU_REPEAT_7(x)  MU_REPEAT_6(x) , MU_ENUM(x,7)
#define MU_REPEAT_8(x)  MU_REPEAT_7(x) , MU_ENUM(x,8)
#define MU_REPEAT_9(x)  MU_REPEAT_8(x) , MU_ENUM(x,9)
#define MU_REPEAT_10(x) MU_REPEAT_9(x) , MU_ENUM(x,10)
#define MU_REPEAT_11(x) MU_REPEAT_10(x) , MU_ENUM(x,11)
#define MU_REPEAT_12(x) MU_REPEAT_11(x) , MU_ENUM(x,12)
#define MU_REPEAT_13(x) MU_REPEAT_12(x) , MU_ENUM(x,13)
#define MU_REPEAT_14(x) MU_REPEAT_13(x) , MU_ENUM(x,14)
#define MU_REPEAT_15(x) MU_REPEAT_14(x) , MU_ENUM(x,15)
#define MU_REPEAT_16(x) MU_REPEAT_15(x) , MU_ENUM(x,16)
#define MU_REPEAT_17(x) MU_REPEAT_16(x) , MU_ENUM(x,17)
#define MU_REPEAT_18(x) MU_REPEAT_17(x) , MU_ENUM(x,18)
#define MU_REPEAT_19(x) MU_REPEAT_18(x) , MU_ENUM(x,19)
#define MU_REPEAT_20(x) MU_REPEAT_19(x) , MU_ENUM(x,20)

#define MU_REPEAT_2_1(x,y)  MU_ENUM(x,1)  MU_ENUM(y,1) 
#define MU_REPEAT_2_2(x,y)  MU_REPEAT_2_1(x,y) ,  MU_ENUM(x,2)    MU_ENUM(y,2)   
#define MU_REPEAT_2_3(x,y)  MU_REPEAT_2_2(x,y) ,  MU_ENUM(x,3)    MU_ENUM(y,3)
#define MU_REPEAT_2_4(x,y)  MU_REPEAT_2_3(x,y) ,  MU_ENUM(x,4)    MU_ENUM(y,4)
#define MU_REPEAT_2_5(x,y)  MU_REPEAT_2_4(x,y) ,  MU_ENUM(x,5)    MU_ENUM(y,5)
#define MU_REPEAT_2_6(x,y)  MU_REPEAT_2_5(x,y) ,  MU_ENUM(x,6)    MU_ENUM(y,6)
#define MU_REPEAT_2_7(x,y)  MU_REPEAT_2_6(x,y) ,  MU_ENUM(x,7)    MU_ENUM(y,7)
#define MU_REPEAT_2_8(x,y)  MU_REPEAT_2_7(x,y) ,  MU_ENUM(x,8)    MU_ENUM(y,8)
#define MU_REPEAT_2_9(x,y)  MU_REPEAT_2_8(x,y) ,  MU_ENUM(x,9)    MU_ENUM(y,9)
#define MU_REPEAT_2_10(x,y) MU_REPEAT_2_9(x,y) ,  MU_ENUM(x,10)   MU_ENUM(y,10)
#define MU_REPEAT_2_11(x,y) MU_REPEAT_2_10(x,y) , MU_ENUM(x,11)   MU_ENUM(y,11)
#define MU_REPEAT_2_12(x,y) MU_REPEAT_2_11(x,y) , MU_ENUM(x,12)   MU_ENUM(y,12)
#define MU_REPEAT_2_13(x,y) MU_REPEAT_2_12(x,y) , MU_ENUM(x,13)   MU_ENUM(y,13)
#define MU_REPEAT_2_14(x,y) MU_REPEAT_2_13(x,y) , MU_ENUM(x,14)   MU_ENUM(y,14)
#define MU_REPEAT_2_15(x,y) MU_REPEAT_2_14(x,y) , MU_ENUM(x,15)   MU_ENUM(y,15)
#define MU_REPEAT_2_16(x,y) MU_REPEAT_2_15(x,y) , MU_ENUM(x,16)   MU_ENUM(y,16)
#define MU_REPEAT_2_17(x,y) MU_REPEAT_2_16(x,y) , MU_ENUM(x,17)   MU_ENUM(y,17)
#define MU_REPEAT_2_18(x,y) MU_REPEAT_2_17(x,y) , MU_ENUM(x,18)   MU_ENUM(y,18)
#define MU_REPEAT_2_19(x,y) MU_REPEAT_2_18(x,y) , MU_ENUM(x,19)   MU_ENUM(y,19)
#define MU_REPEAT_2_20(x,y) MU_REPEAT_2_19(x,y) , MU_ENUM(x,20)   MU_ENUM(y,20)

#define MU_REPEAT(x,n) MU_REPEAT_ ## n(x)
#define MU_REPEAT_2(x,y,n) MU_REPEAT_2_ ## n(x,y)

#endif /* _MACRO_UTIL_H_ */

